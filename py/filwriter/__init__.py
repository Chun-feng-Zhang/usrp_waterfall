from ctypes import *
import os
PATH, this_file = os.path.split(os.path.realpath(__file__))
filheader = CDLL(PATH + "/filheader.so")


def seven_beam_dump(fout, source_name, ibeam, nbeams, tstart, curr_time, az_start, za_start, src_raj, src_dej):
    #source_name = "BACKEND_TEST_BEAM0"
    machine_id = 8 
    telescope_id = 21
    nchans = 512 
    nbits = 32
    #nbeams = 7 
    tsamp = c_double(54.61333e-6)
    #fch1 = c_double(1510.)
    fch1 = c_double(1210.5859375)
    foff = c_double(0.5859375)
    #curr_time = time.time() + time.timezone
    filheader.write_header(py_object(fout), source_name, machine_id, telescope_id, nchans, nbits, c_int(nbeams), c_int(ibeam), c_double(tstart), c_double(curr_time), tsamp, fch1, foff, c_double(az_start), c_double(za_start), c_double(src_raj), c_double(src_dej))
    return None
