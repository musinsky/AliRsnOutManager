#!/bin/bash

SCREENNAME=$(basename $(pwd))
echo $SCREENNAME
screen -dmS $SCREENNAME bash -c $(pwd)/run.sh
