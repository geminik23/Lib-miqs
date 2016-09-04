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
		phasor() = default;
		phasor(double freq, uint32_t sr):frequency{ freq }, samplerate{ sr } { update(); reset(); }


		void set_frequency(double freq) { this->frequency = freq; }
		void set_samplerate(uint32_t sr) { this->samplerate = sr; }
		double get_phase() const noexcept { return phase; }

		void reset() noexcept { phase = 0.0; }
		void update() noexcept { step = miqs::PI * 2 * frequency / samplerate; }
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

	// phasor index
	struct phasor_index
	{
		phasor_index() = default;
		phasor_index(size_t index):index{ index } {}

		void set_index(size_t index) noexcept { this->index = index; }
		size_t get_index() const noexcept { return index; }

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
		phasor_bounded_index() = default;
		phasor_bounded_index(size_t bound, size_t index = 0):bound_index{ bound }, index{ index } {}

		void set_bound(size_t bound) noexcept { this->bound_index = bound; }
		void set_index(size_t index) noexcept { this->index = index; }
		size_t get_index() const noexcept { return index; }
		size_t get_bound() const noexcept { return bound_index; }

		void reset() noexcept { index = 0; }
		void update() noexcept { index %= bound_index; }
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

}