#################################################################################
#  INTEL CONFIDENTIAL
#
#  Copyright (c) 2022 Intel Corporation
#  All Rights Reserved.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit this
#  software or the related documents without Intel's prior written permission.
#
#  This software and the related documents are provided as is, with no express or
#  implied warranties, other than those that are expressly stated in the License.
#################################################################################

import builtins
from ctypes import *

CTYPES_MAPPING = {
    "bool": ("c_bool", c_bool),
    "enum": ("c_int", c_int),
    "int": ("c_int", c_int),
    "double": ("c_double", c_double),
    "string": ("c_char_p", c_char_p)
}


class param_set(Structure):
    _fields_ = [("name", c_char_p),
                ("type", c_char_p),
                ("defaults", c_char_p)]


class result_set(Structure):
    _fields_ = [("header", c_char_p),
                ("unit", c_char_p),
                ("type", c_char_p)]


class env_param(Structure):
    pass


class env_description(Structure):
    pass


class enum_description(Structure):
    _fields_ = [("enum_name", c_char_p),
                ("enum_desc", c_char_p)]


class test_description(Structure):
    pass


class test_results(Structure):
    pass


def print_banner(msg, width=66):
    fmt_msg = f" {msg} ".ljust(int((width + len(msg) + 2) / 2), "*").rjust(width, "*")
    print(f"\n{fmt_msg}\n")


def cast_param(value, cast_type):
    if cast_type == "enum":
        cast_type = "int"
    try:
        defaults = getattr(builtins, cast_type)(value)
    except ValueError:
        print(f"\nCould not cast value {value} into type {cast_type}. Returning "
              f"default value for type {cast_type}\n")
        defaults = 0 if cast_type == "int" else 0.0
    return defaults


def cast_to_ctypes(python_type_name):
    return CTYPES_MAPPING[python_type_name][1]


def get_csv_file_name(name):
    return f"perf_{name}.csv"
