#include "util/pack/lzss.h"
#include "util/range.h"

using namespace au;

bstr au::util::pack::lzss_decompress(
    const bstr &input, size_t orig_size, const LzssSettings &settings)
{
    io::BitReader bit_reader(input);
    return lzss_decompress(bit_reader, orig_size, settings);
}

bstr au::util::pack::lzss_decompress(
    io::BitReader &bit_reader, size_t orig_size, const LzssSettings &settings)
{
    bstr output;
    output.reserve(orig_size);
    size_t dictionary_size = 1 << settings.position_bits;
    size_t dictionary_pos = settings.initial_dictionary_pos;
    std::unique_ptr<u8[]> dictionary(new u8[dictionary_size]);

    u8 *dictionary_ptr = dictionary.get();

    while (output.size() < orig_size)
    {
        if (bit_reader.get(1) > 0)
        {
            u8 byte = bit_reader.get(8);
            output += byte;
            dictionary_ptr[dictionary_pos] = byte;
            dictionary_pos++;
            dictionary_pos %= dictionary_size;
        }
        else
        {
            unsigned int pos = bit_reader.get(settings.position_bits);
            unsigned int size = bit_reader.get(settings.size_bits);
            size += settings.min_match_size;
            for (auto i : util::range(size))
            {
                u8 byte = dictionary_ptr[pos];
                pos += 1;
                pos %= dictionary_size;

                if (settings.reuse_compressed)
                {
                    dictionary_ptr[dictionary_pos] = byte;
                    dictionary_pos++;
                    dictionary_pos %= dictionary_size;
                }
                output += byte;
                if (output.size() >= orig_size)
                    break;
            }
        }
    }

    return output;
}
