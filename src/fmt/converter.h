#pragma once

#include "transformer.h"

namespace au {
namespace fmt {

    class Converter : public Transformer
    {
    public:
        virtual ~Converter();
        virtual void register_cli_options(ArgParser &) const override;
        virtual void parse_cli_options(const ArgParser &) override;
        virtual FileNamingStrategy get_file_naming_strategy() const override;
        virtual void unpack(File &, FileSaver &, bool) const override;
        std::unique_ptr<File> decode(File &) const;

    protected:
        virtual std::unique_ptr<File> decode_internal(File &) const = 0;
    };

} }
