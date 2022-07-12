/* vim: set expandtab shiftwidth=2 softtabstop=2 tw=70: */

#include <Rcpp.h>
using namespace Rcpp;

// Cross-reference work:
// 1. update ../src/registerDynamicSymbol.c with an item for this
// 2. main code should use the autogenerated wrapper in ../R/RcppExports.R

// References
//
// 1. Nortek AS. “Signature Integration 55|250|500|1000kHz (Version
// Version 2022.2).” Nortek AS, March 31, 2022.


// The next items are specific to ad2cp, as is the whole
// format, but I want to define these here to make the
// code clearer. See [1 sec 6.1].
#define SYNC 0xA5
//#define HEADER_SIZE 10 // but can't this be 12 sometimes? (See below.)
#define FAMILY 0x10

// allowed: 0x15-0x18, ox1a-0x1f, 0xa0
// allowed: 21-24, 26-31, 160
#define NID_ALLOWED 11
int ID_ALLOWED[NID_ALLOWED]={21, 22, 23, 24, 26, 27, 28, 29, 30, 31, 160};

// #define numberKnownIds 5
// const int ids[numberKnownIds] = {0xa0, 0x15, 0x16, 0x17, 0x18};
// const char *meanings[numberKnownIds] = {"String", "Burst Data", "Average Data", "Bottom-Track", "Interleaved Burst"};
// const char *unknownString = "unknown";
//
// const char *id_meaning(int code)
// {
//   for (int i=0; i < numberKnownIds; i++) {
//     if (code == ids[i]) {
//       return(meanings[i]);
//     }
//   }
//   return(unknownString);
// }

/*

   Locate (header+data) for Nortek ad2cp

   @param filename character string indicating the file name.

   @param from integer giving the index of the first ensemble (AKA
   profile) to retrieve. The R notation is used, i.e. from=1 means the
   first profile.

   @param to integer giving the index of the last ensemble to retrieve.
   As a special case, setting this to 0 will retrieve *all* the data
   within the file.

   @param by integer giving increment of the sequence, as for seq(), i.e.
   a value of 1 means to retrieve all the profiles, while a value of 2
   means to get every second profile.

   @value a list containing 'index', 'length' and 'id'. The last of
   these indicates the type of data record (see the table below).

   @examples

   system("R CMD SHLIB ldc_ad2cp_in_file.c")
   f <- "/Users/kelley/Dropbox/oce_ad2cp/labtestsig3.ad2cp"
   dyn.load("ldc_ad2cp_in_file.so")
   a <- .Call("ldc_ad2cp_in_file", f, 1, 10, 1)

@section: notes

Table 6.1 (header definition).  In <> I have the byte number wrt start

+-----------------+-------------------+----------------------------------------------------------+
| Sync            | <0> 1 byte        | Always 0xA5                                              |
+-----------------|-------------------|--------------------------------------------------------+
| Header Size     | <1> 1 byte        | Size (number of bytes) of the Header (10 or 12)        |
+-----------------|-------------------|--------------------------------------------------------+
| ID              | <2> 1 byte        | Defines type of the following Data Record              |
|                 |                   | 0x15 - Burst Data Record.                              |
|                 |                   | 0x16 - Average Data Record.                            |
|                 |                   | 0x17 - Bottom Track Data Record.                       |
|                 |                   | 0x18 - Interleaved Burst Data Record (beam 5).         |
|                 |                   | 0xA0 - String Data Record, eg. GPS NMEA data,          |
|                 |                   |        comment from the FWRITE command.                |
|                 |                   | 0x1A - Burst Altimeter Raw Record.                     |
|                 |                   | 0x1B - DVL Bottom Track Record.                        |
|                 |                   | 0x1C - Echo Sounder Record.                            |
|                 |                   | 0x1D - DVL Water Track Record.                         |
|                 |                   | 0x1E - Altimeter Record.                               |
|                 |                   | 0x1F - Avg Altimeter Raw Record.                       |
|                 |                   | 0x23 - EchoSounder raw sample data record              |
|                 |                   | 0x24 - " " synthetic transmit pulse data record        |
+-----------------|-------------------|--------------------------------------------------------|
| Family          | <3> 1 byte        | Defines the Instrument Family. 0x10 – AD2CP Family     |
+-----------------|-------------------|--------------------------------------------------------+
| Data Size       | <4> 2 or 4 bytes  | Size (number of bytes) of the following Data Record.   |
|                 |                   | 2 bytes if 10-byte header or 4 bytes if 12-byte header |
+-----------------|-------------------|--------------------------------------------------------+
| Data Checksum   | <6 or 8> 2 bytes  | Checksum of the following Data Record.                 |
+-----------------|-------------------|--------------------------------------------------------+
| Header Checksum | <8 or 10> 2 bytes | Checksum of all fields of the Header                   |
|                 |                   | (excepts the Header Checksum itself).                  |
+-----------------+-------------------+--------------------------------------------------------+

Note that the code examples in [1] suggest that the checksums are also unsigned, although
that is not stated in the table. I think the same can be said of [2].

@references

1. "Integrators Guide AD2CP_A.pdf", provided to me privately by
(person 1) in early April of 2017.

2. https://github.com/aodn/imos-toolbox/blob/master/Parser/readAD2CPBinary.m

3. Nortek AS. “Signature Integration 55|250|500|1000kHz (Version
    Version 2022.2).” Nortek AS, March 31, 2022.

@author

Dan Kelley

*/

// Check the header checksum.
//
// The code for this differs from that suggested by Nortek,
// because we don't use a specific (msoft) compiler, so we
// do not have access to misaligned_load16(), which I've seen
// in some Nortek code. Besides, R does not have a 'short' (i.e.
// two-byte) type, so we need work work byte by byte (for the R 'raw'
// type).
//
// At the end of the loop, we check here for an odd number of
// bytes, and zero-pad on the right of the last, if so. This
// is the scheme suggested (in quite different code) at
// https://www.manualslib.com/manual/1595998/Nortek-Signature-Series.html?page=49#manual.
//
// Another site worth checking is
// https://github.com/aodn/imos-toolbox/blob/master/Parser/readAD2CPBinary.m,
// although that does not come from Nortek, and it seems to have no
// provision for a data sequence with an odd number of members.
unsigned short cs(unsigned char *data, unsigned short size, int debug)
{
  unsigned short checksum = 0xB58C;
  if (debug > 1) {
    Rprintf("    %d data: 0x%02x 0x%02x 0x%02x 0x%02x ... 0x%02x 0x%02x 0x%02x 0x%02x\n",
        size, data[0], data[1], data[2], data[3],
        data[size-4], data[size-3], data[size-2], data[size-1]);
  }
  for (int i = 0; i < size; i += 2) {
    checksum += (unsigned short)data[i] + 256*(unsigned short)data[i+1];
  }
  if (1 == size%2) {
    if (debug > 1) {
      Rprintf("    odd # data, so cs changed from 0x%x ", checksum);
    }
    checksum += 256*(unsigned short)data[size-1];
    if (debug > 1) {
      Rprintf("to 0x%x\n", checksum);
    }
  }
  return(checksum);
}


// [[Rcpp::export]]
List do_ldc_ad2cp_in_file(CharacterVector filename, IntegerVector from, IntegerVector to, IntegerVector by, IntegerVector ignoreChecksums, IntegerVector DEBUG)
{
  int debug = DEBUG[0] < 0 ? 0 : DEBUG[0];
  std::string fn = Rcpp::as<std::string>(filename(0));
  FILE *fp = fopen(fn.c_str(), "rb");
  if (!fp)
    ::Rf_error("cannot open file '%s'\n", fn.c_str());

  if (from[0] < 0)
    ::Rf_error("'from' must be positive but it is %d", from[0]);
  //unsigned int from_value = from[0];
  if (to[0] < 0)
    ::Rf_error("'to' must be positive but it is %d", to[0]);
  unsigned int to_value = to[0];
  if (by[0] < 0)
    ::Rf_error("'by' must be positive but it is %d", by[0]);
  //unsigned int by_value = by[0];
  int twelve_byte_header = 0;

  // Find file size, and return to start
  fseek(fp, 0L, SEEK_END);
  long long int filesize = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  if (debug) {
    Rprintf("do_ldc_ad2cp_in_file(filename, from=%d, to=%d, by=%d) {\n", from[0], to[0], by[0]);
    Rprintf("  filename=\"%s\"\n", fn.c_str());
    Rprintf("  filesize=%d bytes\n", filesize);
  }
  long long int chunk = 0;
  long long int cindex = 0;//, cindex_last_good = 0;
  int checksum_failures = 0;

  // Ensure that the first byte we point to equals SYNC.  In a
  // conventional file, starting with a SYNC char, this just gets a
  // byte and puts it back, leaving cindex=0.  But if the file does
  // not start with a SYNC char, e.g.  if the file is a fragment of a
  // larger file, we first step through the file until we find a SYNC,
  // setting cindex appropriately.
  int c;
  while (1) {
    c = getc(fp);
    if (c == EOF) {
      ::Rf_error("this file does not contain a single 0x", SYNC, " byte");
      break;
    }
    if (SYNC == c) {
      fseek(fp, -1, SEEK_CUR);
      break;
    }
    cindex++;
  }
  // The table in [ref 1 sec 6.1, page 80-81] says header pieces are
  // 10 bytes long, so once we get an 0xA5, we'll read 9 more
  // bytes to assemble the header in bytes10.  (We grab all the
  // bytes at once, so we can do a checksum.)
  unsigned char header_bytes[12]; // to store either 10 or 12-byte headers
  struct header {
    unsigned char sync;
    unsigned char header_size;
    unsigned char id;
    unsigned char family;
    unsigned long data_size; // may be 2 bytes or 4 bytes in header
    unsigned short data_checksum;
    unsigned short header_checksum;
  } header;
  unsigned int dbuflen = 10000; // may be increased later
  unsigned char *dbuf = (unsigned char *)R_Calloc((size_t)dbuflen, unsigned char);
  unsigned int nchunk = 100000;
  unsigned int *index_buf = (unsigned int*)R_Calloc((size_t)nchunk, unsigned int);
  unsigned int *length_buf = (unsigned int*)R_Calloc((size_t)nchunk, unsigned int);
  unsigned int *id_buf = (unsigned int*)R_Calloc((size_t)nchunk, unsigned int);
  int early_EOF = 0;
  int reset_cindex = 0; // set to 1 if we skipped to find a new header start, after a bad checksum
  while (chunk < to_value && cindex < filesize) { // FIXME: use whole file here
    if (checksum_failures > 100)
      ::Rf_error("more than 100 checksum errors");
    if (chunk > nchunk - 1) {
      if (debug)
        Rprintf("  increasing 'index_buf' size from %d", nchunk);
      nchunk = (unsigned int) floor(chunk * 1.4); // increase buffer size by sqrt(2)
      index_buf = (unsigned int*)R_Realloc(index_buf, nchunk, unsigned int);
      length_buf = (unsigned int*)R_Realloc(length_buf, nchunk, unsigned int);
      id_buf = (unsigned int*)R_Realloc(id_buf, nchunk, unsigned int);
      if (debug)
        Rprintf("  to %d\n", nchunk);
    }
    size_t bytes_read;
    if (12 != fread(&header_bytes, 1, 12, fp))
      ::Rf_error("cannot read header_bytes at cindex=%ld of %ld byte file\n", cindex, filesize);
    // if (1 != fread(&header.sync, 1, 1, fp))
    //   ::Rf_error("cannot read header.sync at cindex=%ld of %ld byte file\n", cindex, filesize);
    header.sync = header_bytes[0];
    if (header.sync != SYNC)
      ::Rf_error("expected header.sync to be 0x%02x but it was 0x%02x at cindex=%ld (%7.4f%% through file) ... skipping to next 0x%02x character...\n", SYNC, header.sync, cindex, 100.0*cindex/filesize, SYNC);
    header.header_size = header_bytes[1];
    header.id = header_bytes[2];
    header.family = header_bytes[3];
    unsigned char family = header.family; // used in recovery attempt, if a checksum error occurs
    if (header.header_size == 10) {
      header.data_size = header_bytes[4] + 256 * header_bytes[5];
      header.data_checksum = header_bytes[6] + 256 * header_bytes[7];
      header.header_checksum = header_bytes[8] + 256 * header_bytes[9];
      // Give 2 bytes back, since we read 12 and only need 10
      fseek(fp, -2, SEEK_CUR);
    } else if (header.header_size == 12) {
      twelve_byte_header = 1;
      header.data_size = header_bytes[4] + 256 * (header_bytes[5] + 256 * (header_bytes[6] + 256 * header_bytes[7]));
      header.data_checksum = header_bytes[8] + 256 * header_bytes[9];
      header.header_checksum = header_bytes[10] + 256 * header_bytes[11];
    } else {
      ::Rf_error("impossible header.header_size %d (should be 10 or 12) at cindex=%ld (%7.4f%% through file)\n",
          header.header_size, cindex, 100.0*(cindex)/filesize);
    }
    if (debug > 1) {
      Rprintf("Chunk %d at cindex=%ld, %.5f%% through file: id=0x%02x=",
          chunk, cindex, 100.0*cindex/filesize, header.id);
      if (header.id == 0xa0) Rprintf("String\n");
      else if (header.id == 0x15) Rprintf("Burst data record\n");
      else if (header.id == 0x16) Rprintf("Average data record\n");
      else if (header.id == 0x17) Rprintf("Bottom-track data record\n");
      else if (header.id == 0x18) Rprintf("Interleaved burst (beam 5) data record\n");
      else if (header.id == 0x1A) Rprintf("Burst altimeter raw record\n");
      else if (header.id == 0x1B) Rprintf("DVL bottom track record\n");
      else if (header.id == 0x1C) Rprintf("Echo Sounder record\n");
      else if (header.id == 0x1D) Rprintf("DVL water track record\n");
      else if (header.id == 0x1E) Rprintf("Altimeter record\n");
      else if (header.id == 0x1F) Rprintf("Avg altimeter raw record\n");
      else if (header.id == 0x23) Rprintf("Echo Sounder raw sample data record\n");
      else if (header.id == 0x24) Rprintf("Echo Sounder raw synthetic transmit pulse data record\n");
      else Rprintf("Unrecognized ID\n");
      Rprintf("  header: header_size=0x%02x=%d ", header.header_size, header.header_size);
      Rprintf("id=0x%x ", header.id);
      Rprintf("family=0x%x ", header.family);
      Rprintf("data_size=%d ", header.data_size);
      Rprintf("data_checksum=%d ", header.data_checksum);
      Rprintf("header_checksum=%d\n", header.header_checksum);
      if (cindex != ftell(fp) - header.header_size)
        Rprintf("Bug: cindex (%ld) is not equal to ftell()-header_size (%d)\n",
            cindex, ftell(fp)-header.header_size);
    }
    // See if header checksum is correct
    unsigned short computed_header_checksum;
    computed_header_checksum = cs(header_bytes, header.header_size-2, debug);
    if (ignoreChecksums[0] > 0 || computed_header_checksum == header.header_checksum) {
      if (debug > 1) {
        Rprintf("    header checksum equals expectation (or ignoringChecksums is TRUE)\n");
      }
    } else {
      checksum_failures++;
      Rprintf("ERROR: header checksum, 0x%02x, disagrees with expectation, 0x%02x, at cindex=%ld.  (Error ignored in this version of oce.)\n",
          computed_header_checksum, header.header_checksum, cindex);
    }
    cindex = cindex + header.header_size;
    index_buf[chunk] = cindex;
    length_buf[chunk] = header.data_size;

    int found = 0;
    for (int idi = 0; idi < NID_ALLOWED; idi++) {
      if (header.id == ID_ALLOWED[idi]) {
        found = 1;
        break;
      }
    }
    if (found == 0)
      Rprintf("warning : ldc_ad2cp_in_file() skipping undocumented header.id=0x%02x at cindex=%ld\n", header.id, cindex);
    id_buf[chunk] = header.id;
    // Check the header checksum.
    // Increase size of data buffer, if required.
    if (header.data_size > dbuflen) { // expand the buffer if required
      if (debug)
        Rprintf("Increasing 'dbuf' size from %d to %d at cindex:%ld (%.4f%%)\n",
            dbuflen, header.data_size, cindex, 100.0*cindex/filesize);
      if (cindex != ftell(fp))
        Rprintf("  *BUG*: cindex=%ld is out of synch with ftell(fp)=%ld\n", cindex, ftell(fp));

      dbuflen = header.data_size;
      dbuf = (unsigned char *)R_Realloc(dbuf, dbuflen, unsigned char);
    }
    // Read the data
    bytes_read = fread(dbuf, 1, header.data_size, fp);
    // Check that we got all the data
    if (bytes_read != header.data_size) {
      Rprintf("warning : ldc_ad2cp_in_file() EOF before end of chunk %ld at cindex=%ld\n",
          chunk+1, cindex-header.header_size);
      break; // give up
    }
    cindex += header.data_size;
    // Compare data checksum to the value stated in the header
    unsigned short dbufcs;
    dbufcs = cs(dbuf, header.data_size, debug);
    if (ignoreChecksums[0] > 0 || dbufcs == header.data_checksum) {
      //cindex_last_good = cindex - header.header_size - header.data_size;
      reset_cindex = 0;
      if (debug > 1) {
        Rprintf("    data checksum equals expectation (or ignoringChecksums is TRUE)\n");
      }
    } else {
      checksum_failures++;
      Rprintf("ERROR: data checksum, 0x%02x, disagrees with expectation, 0x%02x, at cindex=%ld.\n",
          dbufcs, header.data_checksum, cindex);
      if (cindex != ftell(fp))
        Rprintf("  *BUG*: cindex=%ld is out of synch with ftell(fp)=%ld\n", cindex, ftell(fp));

      while (1) {
        c = getc(fp);
        cindex++;
        if (c == EOF) {
          Rprintf("... got to end of file while searching for a sync character (0x%02x)\n", SYNC);
          early_EOF = 1;
          break;
        }
        if (c == SYNC) {
          //unsigned int trial_cindex = cindex; // so we can reset to here if this trial works
          //.Rprintf("... got a sync character (0x%02x) at cindex=%d (%7.4f%% through file)\n",
          //.    SYNC, cindex, 100.0*cindex/filesize);
          // header size (should be 10 or 12)
          int trial_header_size = getc(fp);
          cindex++;
          if (trial_header_size == EOF) {
            Rprintf("    got to end of file while searching for a header-size character at cindex=%ld\n", cindex);
            early_EOF = 1;
            break;
          }
          if (trial_header_size != 10 && trial_header_size != 12) {
            //.Rprintf("    header-size is %d, not 10 or 12 as expected\n", trial_header_size);
            continue;
          }
          //. Rprintf("        proper header-size character (either 10 or 20 decimal)\n");
          // Skip over the id byte, which has many possibilities we know of (and perhaps more),
          // so it is a bit hard to check for correctness.
          c = getc(fp);
          cindex++;
          if (c == EOF) {
            Rprintf("got to end of file while searching for a the 'id' byte\n");
            early_EOF = 1;
            break;
          }
          // family: assume it's the same for the whole file.
          c = getc(fp);
          cindex++;
          if (c == EOF) {
            Rprintf("got to end of file while searching for a the 'family' byte\n");
            early_EOF = 1;
            break;
          }
          if (c == family) {
            //. Rprintf("            family=%d is consistent with previous family\n", family, cindex);
            cindex -= 4;
            fseek(fp, -4, SEEK_CUR);
            Rprintf("   ... skipped forward to a possible header at cindex=%ld\n", cindex);
            if (cindex != ftell(fp))
              Rprintf("  *BUG*: cindex=%ld is out of synch with ftell(fp)=%ld\n", cindex, ftell(fp));
            break;
          }
        }
      }
      if (early_EOF == 1)
        break; // give up on further processing
    }
    if (!reset_cindex) {
      chunk++;
    }
  }
  IntegerVector index(chunk), length(chunk), id(chunk);
  for (unsigned int i = 0; i < chunk; i++) {
    index[i] = index_buf[i];
    length[i] = length_buf[i];
    id[i] = id_buf[i];
  }
  R_Free(index_buf);
  R_Free(length_buf);
  R_Free(id_buf);
  if (debug)
    Rprintf("} # do_ldc_ad2cp_in_file()\n");
  return(List::create(Named("index")=index,
        Named("length")=length,
        Named("id")=id,
        Named("checksumFailures")=checksum_failures,
        Named("earlyEOF")=early_EOF,
        Named("twelve_byte_headered")=twelve_byte_header));
}

