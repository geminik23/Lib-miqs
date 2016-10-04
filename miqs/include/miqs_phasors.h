#pragma once

#include "miqs_basictype.h"

namespace miqs
{
	struct phasor;
	struct phasor_index;
	struct phasor_bounded_index;




	// phasor
	struct phasor
	{
		typedef double result_type;
		phasor() = default;
		phasor(double freq, uint32_t sr):frequency{ freq }, samplerate{ sr } { update(); reset(); }


		void set_frequency(double freq) noexcept{ this->frequency = freq;  update();}
		void set_samplerate(uint32_t sr) noexcept{ this->samplerate = sr;  update();}
		void set(double freq, uint32_t sr) noexcept{ this->frequency = freq; this->samplerate = sr; update(); }
		
		double get_phase() const noexcept { return phase; }
		double& get_phase() noexcept { return phase; }
		void set_phase(double p) noexcept { this->phase = p; }

		void reset(double init=0) noexcept { phase = init; }
		void update() noexcept { step = miqs::Pi * 2 * frequency / samplerate; }
		void next() noexcept { phase += step; }

		double operator()() noexcept
		{
			auto p = get_phase();
			next();
			return p;
		}

		double phase{};
		double step{};

		double frequency{};
		uint32_t samplerate{};
	};

	// phasor using normalized frequency
	struct phasor_norm_freq
	{
		typedef double result_type;

		phasor_norm_freq() = default;
		phasor_norm_freq(double norm_freq):norm_frequency{ norm_freq }{ update(); reset(); }

		void set(double norm_freq) noexcept{ this->norm_frequency = norm_freq; update(); }
		
		double get_phase() const noexcept { return phase; }
		double& get_phase() noexcept { return phase; }
		void set_phase(double p) noexcept { this->phase = p; }

		void reset(double init=0.0) noexcept { phase = init; }
		void update() noexcept { step = miqs::Pi *norm_frequency; }
		void next() noexcept { phase += step; }

		double operator()() noexcept
		{
			auto p = get_phase();
			next();
			return p;
		}

		double phase{};
		double step{};

		double norm_frequency{};
	};

	// phasor index
	struct phasor_index
	{
		typedef size_t result_type;

		phasor_index() = default;
		phasor_index(size_t idx):index{ idx } {}

		void set_index(size_t idx) noexcept { this->index = idx; }
		size_t get_index() const noexcept { return index; }

		void set_phase(size_t idx) noexcept { this->index = idx; }
		size_t get_phase() const noexcept { return index; }
		size_t& get_phase() noexcept { return index; }


		void reset() noexcept { index = 0; }
		void update() noexcept {}
		void next() noexcept { ++index; }

		size_t operator()() noexcept
		{
			return index++;
		}

		size_t index{};
	};

	// phasor bounded index
	struct phasor_bounded_index
	{
		typedef size_t result_type;

		phasor_bounded_index() = default;
		phasor_bounded_index(size_t bound, size_t idx = 0):bound_index{ bound }, index{ idx } {}

		void set_bound(size_t bound) noexcept { this->bound_index = bound; }
		void set_index(size_t idx) noexcept { this->index = idx; }
		size_t get_index() const noexcept { return index; }
		size_t get_bound() const noexcept { return bound_index; }

		void set_phase(size_t idx) noexcept { this->index = idx; }
		size_t get_phase() const noexcept { return index; }
		size_t& get_phase() noexcept { return index; }

		void reset(size_t init = 0) noexcept { index = init; }
		void update() noexcept { if(index > bound_index) index %= bound_index; }
		void next() noexcept { ++index; update(); }

		size_t operator()() noexcept
		{
			auto i = index;
			next();
			return i;
		}

		size_t index{};
		size_t bound_index{4096};
	};



	// phasor using step
	struct phasor_step
	{
		typedef double result_type;

		phasor_step() = default;
		phasor_step(double step_):step{ step_ } { update(); reset(); }

		double get_phase() const noexcept { return phase; }
		void set_phase(double p) noexcept { this->phase = p; }

		double get_step() const noexcept { return step; }
		void set_step(double s) noexcept { this->step = s; }

		void reset(double init = 0.0) noexcept { phase = init; }
		void update() noexcept {}
		void next() noexcept { phase += step; }

		double operator()() noexcept
		{
			auto p = get_phase();
			next();
			return p;
		}

		double phase{};
		double step{};

	};

	// phasor bounded step
	struct phasor_bounded_step
	{
		typedef double result_type;

		phasor_bounded_step() = default;
		phasor_bounded_step(double step_, double bound_):step{ step_ }, bound_phase{ bound_ } { update(); reset(); }

		double get_bound() const noexcept { return this->bound_phase; }
		void set_bound(double bound) noexcept { this->bound_phase = bound; }
		
		double get_phase() const noexcept { return phase; }
		double& get_phase() noexcept { return phase; }
		void set_phase(double p) noexcept { this->phase = p; }

		double get_step() const noexcept { return step; }
		void set_step(double s) noexcept { this->step = s; }

		void reset(double init = 0.0) noexcept { phase = init; }
		void update() noexcept { if (phase >= bound_phase) phase = std::fmod(phase, bound_phase); }
		void next() noexcept { phase += step; }

		double operator()() noexcept
		{
			auto p = get_phase();
			next();
			return p;
		}

		double phase{};
		double step{};
		double bound_phase{};

	};


}