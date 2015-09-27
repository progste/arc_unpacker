#pragma once

#include <string>
#include "fmt/image_decoder.h"

namespace au {
namespace fmt {
namespace touhou {

    class Pak2ImageDecoder final : public ImageDecoder
    {
    public:
        Pak2ImageDecoder();
        ~Pak2ImageDecoder();
        void add_palette(const std::string &name, const bstr &palette_data);
    protected:
        bool is_recognized_internal(File &) const override;
        pix::Grid decode_internal(File &) const override;
    private:
        struct Priv;
        std::unique_ptr<Priv> p;
    };

} } }