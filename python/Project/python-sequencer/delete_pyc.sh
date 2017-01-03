#!/bin/sh
find . -iname "*.pyc" -exec rm '{}' ';'
echo "Success"
