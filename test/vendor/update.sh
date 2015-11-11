#!/bin/sh
alias wget='wget -N -q --progress=bar --show-progress'

cd "$(dirname $0)"
wget 'https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp'
