//========================================================================================
// Athena++ astrophysical MHD code
// Copyright(C) 2014 James M. Stone <jmstone@princeton.edu> and other code contributors
// Licensed under the 3-clause BSD License, see LICENSE file for details
//========================================================================================
//! \file io_wrapper.cpp
//  \brief functions that provide wrapper for MPI-IO versus serial input/output

// C headers
#include <stdio.h>
#include <stdlib.h>

// C++ headers
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

// Athena++ classes headers
#include "io_wrapper.h"

//----------------------------------------------------------------------------------------
//! \fn int IOWrapper::Open(const char* fname, enum rwmode rw)
//  \brief wrapper for {MPI_File_open} versus {fopen} including error check

int IOWrapper::Open(const char* fname, enum rwmode rw) {
  std::stringstream msg;

  if (rw==IO_WRAPPER_READ_MODE) {
    if ((fh_ = fopen(fname,"rb")) == NULL) {
      msg << "### FATAL ERROR in function [IOWrapper:Open]"
          <<std::endl<< "Input file '" << fname << "' could not be opened" <<std::endl;
      throw std::runtime_error(msg.str().c_str());
      return false;
    }

  } else if (rw==IO_WRAPPER_WRITE_MODE) {
    if ((fh_ = fopen(fname,"wb")) == NULL) {
      msg << "### FATAL ERROR in function [IOWrapper:Open]"
          <<std::endl<< "Output file '" << fname << "' could not be opened" <<std::endl;
      throw std::runtime_error(msg.str().c_str());
      return false;
    }
  } else {
    return false;
  }

  return true;
}

//----------------------------------------------------------------------------------------
//! \fn int IOWrapper::Read(void *buf, IOWrapperSize_t size, IOWrapperSize_t count)
//  \brief wrapper for {MPI_File_read} versus {fread}

size_t IOWrapper::Read(void *buf, IOWrapperSize_t size, IOWrapperSize_t count) {
  return fread(buf,size,count,fh_);
}

//----------------------------------------------------------------------------------------
//! \fn int IOWrapper::Read_all(void *buf, IOWrapperSize_t size, IOWrapperSize_t count)
//  \brief wrapper for {MPI_File_read_all} versus {fread}

size_t IOWrapper::Read_all(void *buf, IOWrapperSize_t size, IOWrapperSize_t count) {
  return fread(buf,size,count,fh_);
}

//----------------------------------------------------------------------------------------
//! \fn int IOWrapper::Read_at_all(void *buf, IOWrapperSize_t size,
//                             IOWrapperSize_t count, IOWrapperSize_t offset)
//  \brief wrapper for {MPI_File_read_at_all} versus {fseek+fread}

size_t IOWrapper::Read_at_all(void *buf, IOWrapperSize_t size,
                           IOWrapperSize_t count, IOWrapperSize_t offset) {
  fseek(fh_, offset, SEEK_SET);
  return fread(buf,size,count,fh_);
}

//----------------------------------------------------------------------------------------
//! \fn int IOWrapper::Write(const void *buf, IOWrapperSize_t size, IOWrapperSize_t cnt)
//  \brief wrapper for {MPI_File_write} versus {fwrite}

size_t IOWrapper::Write(const void *buf, IOWrapperSize_t size, IOWrapperSize_t cnt) {
  return fwrite(buf,size,cnt,fh_);
}

//----------------------------------------------------------------------------------------
//! \fn int IOWrapper::Write_at_all(const void *buf, IOWrapperSize_t size,
//                                  IOWrapperSize_t cnt, IOWrapperSize_t offset)
//  \brief wrapper for {MPI_File_write_at_all} versus {fseek+fwrite}.

size_t IOWrapper::Write_at_all(const void *buf, IOWrapperSize_t size,
                            IOWrapperSize_t cnt, IOWrapperSize_t offset) {
  fseek(fh_, offset, SEEK_SET);
  return fwrite(buf,size,cnt,fh_);
}


//----------------------------------------------------------------------------------------
//! \fn void IOWrapper::Close(void)
//  \brief wrapper for {MPI_File_close} versus {fclose}

int IOWrapper::Close(void) {
  return fclose(fh_);
}

//----------------------------------------------------------------------------------------
//! \fn int IOWrapper::Seek(IOWrapperSize_t offset)
//  \brief wrapper for {MPI_File_seek} versus {fseek}

int IOWrapper::Seek(IOWrapperSize_t offset) {
  return fseek(fh_, offset, SEEK_SET);
}

//----------------------------------------------------------------------------------------
//! \fn IOWrapperSize_t IOWrapper::GetPosition(void)
//  \brief wrapper for {MPI_File_get_position} versus {ftell}

IOWrapperSize_t IOWrapper::GetPosition(void) {
  return ftell(fh_);
}
