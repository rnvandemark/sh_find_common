#!/bin/bash

set -e

# Small helper script to make a FIND3 RESTful request
# Three arguments are required:
# - The name of the node hosting the find server parameters
# - The request type (e.g. 'GET')
# - The API call (e.g. 'api/v1/calibrate/FAMILY'), see https://www.internalpositioning.com/doc/api.md
# The smart home workspace must have been sourced already.

if [ 3 -ne $# ]; then echo "Incorrect number of arguments given, expected 3." >&2; exit 1; fi

HOST_PARAM_VALUE="$(ros2 param get "$1" "$(get_ros_name.sh params.FIND_SERVER_HOST)" | sed -e 's|.*: *||g')"
PORT_PARAM_VALUE="$(ros2 param get "$1" "$(get_ros_name.sh params.FIND_SERVER_PORT)" | sed -e 's|.*: *||g')"

http "$2" "http://$HOST_PARAM_VALUE:$PORT_PARAM_VALUE/$3"
