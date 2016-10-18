#pragma once
#include "miqs_basictype.h"

namespace miqs
{
	namespace env
	{

		struct linear
		{
			typedef sample_t result_type;
			typedef double argument_type;

			linear(std::pair<sample_t, sample_t>const& minmax, size_t size):
				m_size{ size }, m_minmax{ minmax } {}

			result_type operator()(argument_type phase)
			{
				double dif = m_size - 1 - phase;
				if (dif <= 0) return m_minmax.second;
				return (m_minmax.second - m_minmax.first)*(phase) / (m_size - 1) + m_minmax.first;
			}

			void set_size(size_t size) { m_size = size; }
			size_t size() { return m_size; }

		private:
			std::pair<sample_t, sample_t> m_minmax;
			size_t m_size;
		};

		struct exponential
		{
			typedef sample_t result_type;
			typedef double argument_type;

			exponential(std::pair<sample_t, sample_t>const& minmax, size_t size):
				m_size{ size }, m_minmax{ minmax } {}

			result_type operator()(argument_type phase)
			{
				double dif = m_size - 1 - phase;
				if (dif <= 0) return m_minmax.second;
				return static_cast<result_type>(m_minmax.first * std::pow(m_minmax.second / m_minmax.first, phase));
			}

			void set_size(size_t size) { m_size = size; }
			size_t size() { return m_size; }

		private:
			std::pair<sample_t, sample_t> m_minmax;
			size_t m_size;
		};


		struct adsr
		{
			typedef sample_t result_type;
			typedef double argument_type;

			adsr(double max_amp, std::tuple<sample_t, sample_t, sample_t> const& adsr, double amp_sus, size_t size):
				m_maxamp{ max_amp }, m_size{ size }, m_adsr{ adsr }, m_amp_sus{ amp_sus } {}


			result_type operator()(argument_type phase)
			{
				double dif = m_size - 1 - phase;
				if (dif <= 0) return static_cast<result_type>(0);
				if (phase <= std::get<0>(m_adsr))
				{
					return phase*(m_maxamp / std::get<0>(m_adsr));
				}
				else if (phase <= std::get<0>(m_adsr) + std::get<1>(m_adsr))
				{
					return ((m_amp_sus - m_maxamp) / std::get<1>(m_adsr))*(m_size - std::get<0>(m_adsr)) + m_maxamp;
				}
				else if (phase <= m_size - std::get<2>(m_adsr))
					return m_amp_sus;
				return -(m_amp_sus / std::get<2>(m_adsr))*(phase - (m_size - std::get<2>(m_adsr))) + m_amp_sus;
			}

			void set_size(size_t size) { m_size = size; }
			size_t size() { return m_size; }

		private:
			std::tuple<sample_t, sample_t, sample_t> m_adsr;
			double m_maxamp;
			double m_amp_sus;
			size_t m_size;
		};



	}




}