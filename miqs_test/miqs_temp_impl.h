#pragma once

#include <miqs.h>

namespace miqs
{
	
	namespace module
	{
		class process_proxy
		{
		public:
			void fill_silence_all() noexcept{ 
				size_t j{};
				for (size_t i{}; i < m_nout; ++i)
				{
					for (j = 0; j < m_nframe; ++j)
					{
						m_signal_o[i][j] = 0.0;
					}
				}
			}

			void copy_io(size_t i, size_t o) noexcept{
				for (size_t n{}; n < m_nframe; ++n)
				{
					m_signal_o[o][n] = m_signal_i[i][n];
				}
			}
			
			size_t frame_count() const noexcept { return m_nframe; }
			size_t input_count() const noexcept { return m_nin; }
			size_t output_count() const noexcept { return m_nout; }
			double samplerate() const noexcept { return m_samplerate; }

			template <typename _Func>
			void perform(_Func function)
			{
				function(*this, m_signal_i, m_nin, m_signal_o, m_nout, m_nframe);
			}

		private:
			sample_t ** m_signal_i, **m_signal_o;
			size_t m_nin, m_nout;
			size_t m_nframe;
			double m_samplerate;
		};

		struct processor
		{
			virtual void process(process_proxy&, double samplerate, double nframe) = 0;
		protected:
			virtual void perform(process_proxy& proxy, sample_t ** in, size_t nin, sample_t ** out, size_t nout, size_t nframe) {}
		};


	}

	



	template <typename _InIt, typename _OutIt, typename _ContIdx>
	void copy_index(_InIt first, _OutIt dst, _ContIdx const& cont)
	{
		for (auto i : cont) *dst++ = *(first + i);
	}

	inline double phase_wrapping(double in)
	{
		return std::remainder(in + miqs::Pi, -2 * miqs::Pi) + Pi;
	}


	template <typename _InIt, typename _OutIt>
	_OutIt pitch_track_yin(_InIt first, _InIt last, _OutIt dst, std::uint32_t samplerate, double f0min, size_t hopsize)
	{

		double yinTolerance = 0.22;
		double temp{};
		size_t taumax = static_cast<size_t>(1 / f0min*fs + 0.5);
		size_t yinLen = 1024;

		size_t sample_size = std::distance(first, last);

		size_t i{}, j{};
		size_t pin{};
		size_t pend{ sample_size - (yinLen + taumax) + 1 };

		std::vector<sample_t> xframe((yinLen + taumax) + 1);
		std::vector<sample_t> yintemp(taumax);

		auto sbegin = first;
		auto send = sbegin + (yinLen + taumax + 1);
		for (; pin <= pend; pin += hopsize, dst++, sbegin += hopsize, send += hopsize)
		{
			std::copy(sbegin, send, std::begin(xframe));
			std::fill(std::begin(yintemp), std::end(yintemp), 0.0);

			// calculate the square differences
			for (i = 0; i < taumax; ++i)
			{
				for (j = 0; j < yinLen; ++j)
				{
					yintemp[i] = yintemp[i] + std::pow((xframe[j] - xframe[j + i]), 2);
				}
			}

			// calculate cumulated normalization
			temp = 0.0;
			yintemp[0] = 1.0;

			for (i = 1; i < taumax; ++i)
			{
				temp = temp + yintemp[i];
				yintemp[i] = yintemp[i] * (i*temp);
			}

			//determine lowest pitch
			i = 0;
			while (i < taumax)
			{
				if (yintemp[i] < yinTolerance)
				{
					while (yintemp[i + 1] < yintemp[i])
					{
						++i;
					}
					*dst = (double)samplerate / i;
					break;
				}
				else ++i;

				*dst = 0.0;
			}
		}

		return dst;

	}

	template <typename _InIt1, typename _InIt2,
		typename _ResultRIt, typename _ResultRNormIt, typename _ResultR0It>
		std::tuple<_ResultRIt, _ResultRNormIt, _ResultR0It> xcorrelation_normalized(_InIt1 lbegin, _InIt1 lend, _InIt2 rbegin,
																					_ResultRIt rxx, _ResultRNormIt rxx_norm, _ResultR0It rxx0,
																					size_t lmin, size_t lmax)
	{
		size_t nblock = std::distance(lbegin, lend);
		size_t nlag = lmax - lmin + 1;
		auto prxx = rxx;
		auto prxx0 = rxx0;

		for (size_t i{}; i < nlag; ++i, ++prxx, ++prxx0)
		{
			auto xp_b = lbegin + (int)(lmax - i);
			size_t lag = i + lmin;
			*prxx0 = std::inner_product(xp_b, xp_b + nblock, xp, 0.0);
			*prxx1 = std::inner_product(lbegin, lend, xp_b, 0.0);
		}
		std::transform(rxx, rxx + nlag, rxx0, rxx_norm, [](auto&&l, auto&&r) {
			return l*l / r;
		});

		return{ rxx + nlag, rxx_norm + nlag, rxx0 + nlag };
	}


	// input signal is lmax presamples signal  x.size() = lmax + nblock
	template <typename _InIt, typename _ResultLagIt, typename _ResultFreqIt>
	std::pair<_ResultLagIt, _ResultFreqIt> pitch_track_ltp(_InIt first, _InIt last,
														   _ResultLagIt dst_lag, _ResultFreqIt dst_freq,
														   size_t lmin, size_t lmax,
														   size_t nblock, std::uint32_t samplerate,
														   double b0_thres)
	{
		find_max_locations<sample_t> fml;
		size_t lag_size{ lmax - lmin + 1 };
		std::vector<size_t> lags(lag_size);
		std::vector<size_t> idx;
		std::vector<double>
			rxx_norm(lag_size), // normalized autocorr 
			rxx(lag_size),  // autocorr. sequence
			rxx0(lag_size), // energy of delayed blocks
			temp(lag_size);

		miqs::xcorrelation_normalized(first, first + nblock, first + lmax, std::begin(rxx), std::begin(rxx_norm), std::begin(rxx0), lmin, lmax);

		// calcuate
		std::transform(std::begin(rxx), std::end(rxx), std::begin(rxx0), std::begin(temp), std::divides<sample_t>());
		fml(std::begin(rxx), std::end(rxx));

		idx.reserve(fml.idx.size());
		for (auto&& i : fml.idx)
		{
			if (rxx[i] > 0 && std::abs(temp[i] - 1.0) < b0_thres)
			{
				*dst_lag++ = this->lags[i];
				*dst_freq++ = (double)samplerate / this->lags[i];
			}
		}

		return{ dst_lag, dst_freq }
	}



	template <typename _Ty>
	struct find_max_locations
	{
		typedef _Ty value_type;

		template <typename _InIt, typename _Ty = std::iterator_traits<_InIt>::value_type>
		void operator()(_InIt first, _InIt last)
		{
			size_t size = std::distance(first, last);
			size_t i{};
			size_t s_df = size - 2;
			std::vector<_Ty> s(size);

			//resize
			diff.resize(size);
			dx1.resize(s_df);
			dx2.resize(s_df);
			prod.resize(s_df);

			idx.reserve(size);
			idx0.reserve(size);
			idx1.reserve(size);
			idx2.reserve(size);
			idx3.reserve(size);
			idx4.reserve(size);



			// if(idx>)
			idx.clear();
			idx0.clear();
			idx1.clear();
			idx2.clear();
			idx3.clear();
			idx4.clear();



			// from input signal => std::adjacent_difference
			std::adjacent_difference(first, last, std::begin(diff));
			diff.pop_front();

			std::copy(std::begin(diff) + 1, std::end(diff) - 1, std::begin(dx1));
			std::copy(std::begin(diff), std::end(diff) - 2, std::begin(dx2));
			std::transform(std::begin(dx1), std::end(dx1), std::begin(dx2), std::begin(prod), std::multiplies<sample_t>());

			for (i = 0; i < s_df; ++i)	if (prod[i] < 0.0) idx1.push_back(i); // sign changed in idx1
			for (auto&& _i : idx1) if (dx1[_i] < 0.0) idx1.push_back(_i); // only change from + to -
			for (auto&& _i : idx2) idx.push_back(idx1[_i] + 1);

			// check zero in dx => maxima with 2 identical values
			for (i = 0; i < s_df; ++i)	if (diff[i] == 0.0) idx3.push_back(i); // sign changed in idx1
			for (auto&& _i : idx3) if (*(first + _i) > 0) idx4.push_back(_i);
			for (auto&&_i : idx4) idx0.push_back(idx3[_i]);
		}

		std::deque<value_type> diff;
		std::vector<value_type> dx1, dx2, prod;
		std::vector<size_t> idx, idx0, idx1, idx2, idx3, idx4;
	};




}
