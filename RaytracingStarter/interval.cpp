#include "interval.h"

const interval interval::empty = interval(+rt_infinity, -rt_infinity);
const interval interval::universe = interval(-rt_infinity, +rt_infinity);
