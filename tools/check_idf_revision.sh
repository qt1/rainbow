#!/bin/bash
echo components/arduino revision is:
git -C components/arduino rev-parse --short HEAD
( cd components/arduino; git log -1 )

echo
echo arduino revision and esp-idf revision should match:
git -C $IDF_PATH rev-parse --short HEAD
