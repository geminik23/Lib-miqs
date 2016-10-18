#pragma once
#include "miqs_basictype.h"

namespace miqs
{



	//======================Phase info

	template <typename _Ty = double>
	struct phase_info
	{

		typedef _Ty value_type;
		typedef phase_info<value_type> self_type;

		phase_info() = default;
		phase_info(value_type p, value_type inc):phase{ p }, step{ inc } {}

		void reset() noexcept { this->phase = 0; }
		inline void next() { this->phase += step; }
		operator value_type() { return this->phase; }
		self_type& operator+=(value_type v) { this->phase += v; return *this; }
		self_type& operator=(value_type v) { this->phase = v; return *this; }

		value_type phase{};
		value_type step{};
	};

	template <typename _PhaseInfo>
	struct phase_helper
	{
		typedef _PhaseInfo info_type;
		typedef typename info_type::value_type value_type;
		typedef phase_helper<info_type> self_type;

		phase_helper(info_type& pinfo):phase{ pinfo }, samplerate{ 44100 } {}

		void reset() noexcept { phase.reset(); }
		inline void next() noexcept { this->phase.phase += this->phase.step; }
		operator value_type() { return this->phase; }

		void set_samplerate(double sr) noexcept { samplerate = sr; }
		void set_freq(double freq) { phase.step = freq* miqs::two_pi / samplerate; }
		void set_info(double freq, double sr) { this->samplerate = sr; phase.step = freq* miqs::two_pi / samplerate; }

		info_type& phase;
		double samplerate;
	};


	template <typename _PhaseInfo>
	struct phase_operator
	{
		typedef _PhaseInfo info_type;
		typedef typename info_type::value_type value_type;

		phase_operator(info_type& pinfo):phase{ pinfo } {}

		void reset() noexcept { phase.reset(); }
		inline void next() noexcept { this->phase.phase += this->phase.step; }
		operator value_type() { return this->phase; }

		value_type operator()() { value_type t = phase.phase; phase.phase += phase.step; return t; }

		info_type& phase;
	};


	template <typename _PhaseInfo>
	struct phase_pi_modular
	{
		typedef _PhaseInfo info_type;
		typedef typename info_type::value_type value_type;

		phase_pi_modular(info_type& pinfo):phase{ pinfo } {}

		void reset() noexcept { phase.reset(); }
		inline void next() noexcept { this->phase.phase += this->phase.step; while (phase.phase >= miqs::two_pi)	phase.phase -= miqs::two_pi;}
		operator value_type() { return this->phase; }
		void bound_check() { while (phase.phase >= miqs::two_pi)	phase.phase -= miqs::two_pi; }


		value_type operator()() {
			value_type t = phase.phase; 
			phase.phase += phase.step; 
			while (phase.phase >= miqs::two_pi)	phase.phase -= miqs::two_pi;
			return t; 
		}

		info_type& phase;
	};


	template <typename _PhaseInfo>
	struct phase_modular
	{
		typedef _PhaseInfo info_type;
		typedef typename info_type::value_type value_type;

		phase_modular(info_type& pinfo, value_type bound_):phase{ pinfo }, bound{ bound_ } {}

		void reset() noexcept { phase.reset(); }
		void next() noexcept { this->phase.phase += this->phase.step; while (phase.phase >= bound)	phase.phase -= bound;}
		void bound_check(){ while (phase.phase >= bound)	phase.phase -= bound; }

		operator value_type() { return this->phase; }

		value_type operator()()
		{
			value_type t = phase.phase;
			phase.phase += phase.step;
			while (phase.phase >= bound)	phase.phase -= bound;
			return t;
		}

		info_type& phase;
		value_type bound;
	};

}