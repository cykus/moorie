/*
 * This file is a part of Moorie
 *
 * Copyright (C) 2007-2010 Moorie Team - http://moorie.pl/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "BinaryStream.h"

#include <cassert>

BinaryStream::BinaryStream(uint8_t* data, uint32_t size, bool ownership)
	: ownership_(ownership),
	  data_(data),
	  size_(size),
	  read_ptr_(0)
{
}

BinaryStream::~BinaryStream() {
	if (ownership_)
		delete [] data_;
}

uint8_t* BinaryStream::readBytes(uint32_t read) {
	assert(read_ptr_ + read < size_);

	uint8_t* result = new uint8_t[read];
	for (uint32_t j = 0; j < read && read_ptr_ < size_; ++j, ++read_ptr_)
		result[j] = data_[read_ptr_];

	return result;
}

uint8_t BinaryStream::readByte() {
	return readData<uint8_t>();
}

int16_t BinaryStream::readInt16() {
	return readData<int16_t>();
}

uint16_t BinaryStream::readUInt16() {
	return readData<uint16_t>();
}

int32_t BinaryStream::readInt32() {
	return readData<int32_t>();
}

uint32_t BinaryStream::readUInt32() {
	return readData<uint32_t>();
}

int64_t BinaryStream::readInt64() {
	return readData<int64_t>();
}

uint64_t BinaryStream::readUInt64() {
	return readData<uint64_t>();
}

std::string BinaryStream::readString() {
	assert(read_ptr_ < (size_ - 1));

	std::string result;
	while (data_[read_ptr_] != '\0' && read_ptr_ < (size_ - 1)) {
		result += static_cast<char>(data_[read_ptr_]);
		++read_ptr_;
	}

	return result;
}

template<typename T>
inline T BinaryStream::readData() {
	assert(read_ptr_ < (size_ - (1 + sizeof(T))));

	T result = 0;
	for (uint32_t i = 0; i < sizeof(T); ++i, ++read_ptr_)
		result |= data_[read_ptr_] << (i * 8); // little-endian
	return result;
}

