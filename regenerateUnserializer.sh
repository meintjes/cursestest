#!/bin/sh

find . -type f -print0 |
xargs -0 egrep "^\s*CREATE_TAG_FOR\([a-zA-Z]+\)\s*\$" |
cut -d '(' -f 2 |
cut -d ')' -f 1 |
awk '
BEGIN {
    print "#ifndef __GET_POINTER_FROM_ARCHIVE_H__"
    print "#define __GET_POINTER_FROM_ARCHIVE_H__"
    print "#include <exception>"
    print "#include <string>"
    print "#include \"Archive.h\""
    print "void* getPointerFromArchive(Archive &ar) {"
    print "  std::string tag;"
    print "  ar & tag;"
    print "  switch (tagize(tag.c_str())) {"
}

{
    print "  CREATE_CASE_FOR(" $1 ")"
}

END {
    print "  CREATE_NONE_CASE"
    print "  default:"
    print "    throw std::runtime_error(\"Invalid serialization tag: \" + tag);"
    print "    return nullptr;"
    print "  }"
    print "}"
    print "#endif"
}
'
