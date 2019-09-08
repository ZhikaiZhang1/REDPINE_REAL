cat $1 | od -v -w4 -tx4 | awk '{print $2;}' |  sed -e '/^$/d' > rps_file
vi -s append_comma_prepend_0x.tcl rps_file

