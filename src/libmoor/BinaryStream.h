#ifndef BINARYSTREAM_H__
#define BINARYSTREAM_H__

#include <string>
#include <stdint.h>

class BinaryStream {
	public:
		BinaryStream(uint8_t* data,
								 uint32_t size,
								 bool ownership = false);
		~BinaryStream();

	public:
		uint8_t* readBytes(uint32_t size);

		uint8_t readByte();
		int16_t readInt16();
		uint16_t readUInt16();
		int32_t readInt32();
		uint32_t readUInt32();
		int64_t readInt64();
		uint64_t readUInt64();
		std::string readString();

	private:
		template<typename T>
		inline T readData();

	private:
		bool ownership_;
		uint8_t* data_;
		uint32_t size_;
		uint32_t read_ptr_;
};

#endif // BINARYSTREAM_H__
