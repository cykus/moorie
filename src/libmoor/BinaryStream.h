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
 
#ifndef BINARYSTREAM_H__
#define BINARYSTREAM_H__

#include <string>
#include <stdint.h>

class BinaryStream {
	public:
		/**
		 * Creates @c BinaryStream object.
		 * @param data Data the stream should manage.
		 * @param size Size of passed data buffer.
		 * @param ownership If buffer should deallocate data on its destruction.
		 */
		BinaryStream(uint8_t* data, uint32_t size, bool ownership = false);
		~BinaryStream();

	public:
		/**
		 * Gets specified amount of bytes from stream.
		 * @attention It allocates required buffer with C++ @c new and passes
		 *            ownership to the caller.
		 * @attention If there is not enough bytes in buffer, assertion is called.
		 * @todo Should we omit it and return buffer with what's left in it ?
		 * @param read Bytes to read.
		 * @return Buffer with read bytes.
		 */
		uint8_t* readBytes(uint32_t read);

		/**
		 * Gets one byte from stream.
		 * @attention If there is not enough bytes in buffer, assertion is called.
		 * @return Read value.
		 */
		uint8_t readByte();

		/**
		 * Gets 16-bit integer from stream.
		 * @attention If there is not enough bytes in buffer, assertion is called.
		 * @return Read value.
		 */
		int16_t readInt16();

		/**
		 * Gets 16-bit unsigned integer from stream.
		 * @attention If there is not enough bytes in buffer, assertion is called.
		 * @return Read value.
		 */
		uint16_t readUInt16();

		/**
		 * Gets 32-bit integer from stream.
		 * @attention If there is not enough bytes in buffer, assertion is called.
		 * @return Read value.
		 */
		int32_t readInt32();

		/**
		 * Gets 32-bit unsigned integer from stream.
		 * @attention If there is not enough bytes in buffer, assertion is called.
		 * @return Read value.
		 */
		uint32_t readUInt32();

		/**
		 * Gets 64-bit integer from stream.
		 * @attention If there is not enough bytes in buffer, assertion is called.
		 * @return Read value.
		 */
		int64_t readInt64();

		/**
		 * Gets 64-bit unsigned integer from stream.
		 * @attention If there is not enough bytes in buffer, assertion is called.
		 * @return Read value.
		 */
		uint64_t readUInt64();

		/**
		 * Gets string from stream.
		 * Reads buffer until it encounters end of string.
		 * @attention Buffer should have at least one byte left otherwise assertion
		 *            is called.
		 * @return Read string.
		 */
		std::string readString();

		/**
		 * Gets number of bytes left to read in stream.
		 * @return Number of remaining bytes.
		 */
		inline uint32_t size() const { return (size_ - (read_ptr_ + 1)); }

	private:
		BinaryStream(const BinaryStream&);
		BinaryStream& operator=(const BinaryStream&);
		
	private:
		/**
		 * Reads data from buffer.
		 * @attention If there is not enough bytes in buffer, assertion is called.
		 * @tparam T Type of returned value. Should be one of integral types.
		 * @return Read value.
		 */
		template<typename T>
		inline T readData();

	private:
		bool      ownership_; ///< Ownership flag.
		uint8_t*  data_;      ///< Integral buffer.
		uint32_t  size_;      ///< Size of the buffer.
		uint32_t  read_ptr_;  ///< Read pointer. Points onto next byte to read.
};

#endif // BINARYSTREAM_H__
