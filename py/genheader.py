from ctypes import *
import os,sys,glob
import time
from filwriter import filheader

hdrname = sys.argv[1]
source_name = sys.argv[2]
mjdtoday = float(sys.argv[3])

def genheader(fout, source_name, tstart, src_raj, src_dej):
    #source_name = "BACKEND_TEST_BEAM0"
    machine_id = 8 
    telescope_id = 21
    nchans = 2048
    nbits = 32
    ibeam = 1
    nbeams = 1 
    tsamp = c_double(nchans/56e6)
    #fch1 = c_double(1510.)
    fch1 = c_double(1400.+56./2)
    foff = c_double(56./2048)
    az_start = 0.
    za_start = 0.
    curr_time = time.time() + time.timezone
    #filheader.write_header(py_object(fout), source_name, machine_id, telescope_id, nchans, nbits, c_int(nbeams), c_int(ibeam), c_double(tstart), c_double(curr_time), tsamp, fch1, foff, c_double(az_start), c_double(za_start), c_double(src_raj), c_double(src_dej))
    filheader.write_header(fout, source_name, machine_id, telescope_id, nchans, nbits, c_int(nbeams), c_int(ibeam), c_double(tstart), c_double(curr_time), tsamp, fch1, foff, c_double(az_start), c_double(za_start), c_double(src_raj), c_double(src_dej))
    return None

ra_vela = 128.836
dec_vela = -45.176
ra_0329 = 53.2475
dec_0329 = 54.5787
#mjdtoday = 60051

#genheader(hdrname, source_name , mjdtoday, ra_vela, dec_vela)
genheader(hdrname, source_name , mjdtoday, ra_0329, dec_0329)
