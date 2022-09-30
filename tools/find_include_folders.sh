#debug
make -d | awk '/Prerequisite/ { if(match($2,"(.*)(/)(.*\\.h)",m)) { c[m[1]]++ ; print m[0] "|" m[1] "|" m[2] "|" m[3] "|" m[4] ;} }' | head

#find all directories with include files that are used in make
make -d | awk '/Prerequisite/ { if(match($2,"(.*)(/)(.*\\.h)",m)) { c[m[1]]++ ; } } END {for(d in c) print d c[d];} ' | sort > include_directories
