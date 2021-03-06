/*
    _____ _____ _____ __
   |   __|     |  |  |  |      The SOUL language
   |__   |  |  |  |  |  |__    Copyright (c) 2019 - ROLI Ltd.
   |_____|_____|_____|_____|

   The code in this file is provided under the terms of the ISC license:

   Permission to use, copy, modify, and/or distribute this software for any purpose
   with or without fee is hereby granted, provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
   TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
   NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
   DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
   IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

namespace soul
{

//==============================================================================
/** A ref-counted holder for a source code string. */
struct SourceCodeText  final : public RefCountedObject
{
    using Ptr = RefCountedPtr<SourceCodeText>;

    static Ptr createForFile (std::string filename, std::string text);
    static Ptr createInternal (std::string name, std::string text);

    const std::string filename, content;
    const UTF8Reader utf8;
    const bool isInternal;

private:
    SourceCodeText() = delete;
    SourceCodeText (const SourceCodeText&) = delete;
    SourceCodeText (std::string, std::string, bool internal);
};


//==============================================================================
/** Represents a source code location as a pointer into a SourceCodeText object. */
struct CodeLocation   final
{
    CodeLocation() noexcept = default;
    CodeLocation (SourceCodeText::Ptr);

    CodeLocation (CodeLocation&&) = default;
    CodeLocation (const CodeLocation&) = default;
    CodeLocation& operator= (CodeLocation&&) = default;
    CodeLocation& operator= (const CodeLocation&) = default;

    /** This is the best way to convert a string to a CodeLocation as it'll also validate the UTF8 and throw
        an error if it's dodgy.
    */
    static CodeLocation createFromString (std::string filename, std::string text);
    static CodeLocation createFromSourceFile (const SourceFile&);
    void validateUTF8() const;

    void emitMessage (CompileMessage) const;
    [[noreturn]] void throwError (CompileMessage) const;

    struct LineAndColumn
    {
        uint32_t line, column;
    };

    bool isEmpty() const;
    std::string getFilename() const;
    LineAndColumn getLineAndColumn() const;
    std::string getSourceLine() const;

    SourceCodeText::Ptr sourceCode;
    UTF8Reader location;
};


} // namespace soul
