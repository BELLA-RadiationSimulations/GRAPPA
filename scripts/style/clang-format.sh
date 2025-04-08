#!/bin/bash

# Function to check the clang-format version
check_clang_format_version() {
    local required_version=18
    local version_output
    local major_version

    # Get the version output from clang-format
    version_output=$(clang-format --version)

    # Extract the major version number
    if [[ $version_output =~ ([0-9]+)\. ]]; then
        major_version=${BASH_REMATCH[1]}
    else
        echo "Unable to determine clang-format version."
        exit 1
    fi

    # Check if the major version is the required version
    if [[ "$major_version" -ne "$required_version" ]]; then
        echo "Error: clang-format version $required_version is required. Found version $major_version."
        exit 1
    fi
}

check_clang_format_version

find src/ include/ scripts/ \
    -type f -name "*.hpp" -o \
    -type f -name "*.cpp" -o \
    -type f -name "*.C.in" -o \
    -type f -name "*.h.in" \
    | xargs clang-format --style=file -i
