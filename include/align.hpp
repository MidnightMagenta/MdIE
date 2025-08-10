#ifndef MDIE_ALIGN_H
#define MDIE_ALIGN_H

namespace mdie {
template<typename t>
t constexpr align_up(t val, t alignment) {
	return (t(val) + (t(alignment) - 1)) & (~(t(alignment) - 1));
}

template<typename t>
t constexpr align_down(t val, t alignment) {
	return t(val) & ~(t(alignment) - 1);
}
}// namespace mdie

#endif// !MDIE_ALIGN_H
