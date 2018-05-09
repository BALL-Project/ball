// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_COMMON_MACROS_H
#define BALL_COMMON_MACROS_H

#include <BALL/COMMON/rtti.h>

#include <typeinfo>  // needed for typeid

#define BALL_DUMP_DEPTH(os, depth)               for (dump_indent_depth_ = 0; dump_indent_depth_ < depth; ++dump_indent_depth_) { os << "    "; }
#define BALL_DUMP_STREAM_PREFIX(os)              Size dump_indent_depth_ = 0;

#define BALL_DUMP_HEADER(os,cl,ob)               os << "Object: " << (void *)ob << " is instance of class: " << streamClassName(typeid(*ob)) << std::endl;
#define BALL_DUMP_CLASS_HEADER(os,cl,ob)         os << "Object: " << (void *)ob << " is instance of class: " << #cl << ::std::endl;
#define BALL_DUMP_STREAM_SUFFIX(os)              

#endif // BALL_COMMON_MACROS_H
