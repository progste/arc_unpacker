#include "fmt/leaf/leafpack_group/lf3_image_decoder.h"
#include "fmt/leaf/common/custom_lzss.h"

using namespace au;
using namespace au::fmt::leaf;

static const bstr magic = "LEAF64K\x00"_b;

bool Lf3ImageDecoder::is_recognized_impl(File &file) const
{
    return file.io.read(magic.size()) == magic;
}

pix::Grid Lf3ImageDecoder::decode_impl(File &file) const
{
    file.io.seek(magic.size());
    file.io.skip(4);
    const auto width = file.io.read_u16_le();
    const auto height = file.io.read_u16_le();

    const auto data_pos = file.io.read_u32_le();

    file.io.seek(data_pos);
    const auto data = common::custom_lzss_decompress(
        file.io.read_to_eof(), width * height * 2);
    pix::Grid image(width, height, data, pix::Format::BGR555X);
    image.flip_vertically();
    return image;
}

static auto dummy = fmt::register_fmt<Lf3ImageDecoder>("leaf/lf3");