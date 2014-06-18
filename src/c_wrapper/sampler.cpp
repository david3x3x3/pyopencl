#include "sampler.h"
#include "context.h"
#include "clhelper.h"

namespace pyopencl {

template class clobj<cl_sampler>;
template void print_arg<cl_sampler>(std::ostream&, const cl_sampler&, bool);
template void print_clobj<sampler>(std::ostream&, const sampler*);
template void print_buf<cl_sampler>(std::ostream&, const cl_sampler*,
                                    size_t, ArgType, bool, bool);

sampler::~sampler()
{
    pyopencl_call_guarded_cleanup(clReleaseSampler, this);
}

generic_info
sampler::get_info(cl_uint param_name) const
{
    switch ((cl_sampler_info)param_name) {
    case CL_SAMPLER_REFERENCE_COUNT:
        return pyopencl_get_int_info(cl_uint, Sampler, this, param_name);
    case CL_SAMPLER_CONTEXT:
        return pyopencl_get_opaque_info(context, Sampler, this, param_name);
    case CL_SAMPLER_ADDRESSING_MODE:
        return pyopencl_get_int_info(cl_addressing_mode, Sampler,
                                     this, param_name);
    case CL_SAMPLER_FILTER_MODE:
        return pyopencl_get_int_info(cl_filter_mode, Sampler, this, param_name);
    case CL_SAMPLER_NORMALIZED_COORDS:
        return pyopencl_get_int_info(cl_bool, Sampler, this, param_name);

    default:
        throw clerror("Sampler.get_info", CL_INVALID_VALUE);
    }
}

}

// c wrapper
// Import all the names in pyopencl namespace for c wrappers.
using namespace pyopencl;

// Sampler
error*
create_sampler(clobj_t *samp, clobj_t _ctx, int norm_coords,
               cl_addressing_mode am, cl_filter_mode fm)
{
    auto ctx = static_cast<context*>(_ctx);
    return c_handle_error([&] {
            *samp = new sampler(pyopencl_call_guarded(clCreateSampler, ctx,
                                                      norm_coords, am, fm),
                                false);
        });
}
