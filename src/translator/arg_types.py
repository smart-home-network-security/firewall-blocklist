"""
Argparse types used by the translator.
"""

import os
import argparse


def uint16(arg: str) -> int:
    """
    Custom type for argparse,
    to check whether an argument is an unsigned 16-bit integer,
    i.e. an integer between 0 and 65535.

    :param arg: given argument
    :return: the argument, as an `int` if it is an unsigned 16-bit integer
    :raises argparse.ArgumentTypeError: if the argument is not an unsigned 16-bit integer
    """
    result = int(arg)
    if result < 0 or result > 65535:
        raise argparse.ArgumentTypeError(f"\"{arg}\" is not an unsigned 16-bit integer (must be between 0 and 65535)")
    return result


def proba(arg: str) -> float:
    """
    Custom type for argparse,
    to check whether an argument is a valid probability,
    i.e. a float between 0 and 1.

    :param arg: given argument
    :return: the argument, as a `float`, if it is a valid probability
    :raises argparse.ArgumentTypeError: if the argument is not a valid probability
    """
    result = float(arg)
    if result < 0 or result > 1:
        raise argparse.ArgumentTypeError(f"\"{arg}\" is not a valid probability (must be a float between 0 and 1)")
    return result


def directory(arg: str) -> str:
    """
    Argparse type for an existing directory path.

    :param arg: given argument
    :return: absolute path to an existing directory
    :raises argparse.ArgumentTypeError: if the given argument is not an existing directory
    """
    if not os.path.isdir(arg):
        raise argparse.ArgumentTypeError(f"\"{arg}\" is not an existing directory")
    return os.path.abspath(arg)
