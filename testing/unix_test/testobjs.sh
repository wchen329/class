# testobjs.sh
# Simple one liner that detect test object files automatically
# wchen329
#
ls ../tests/*.cpp | sed s/.cpp/.o/ | sed s/"..\/tests\/"/" "/ | tr -d \\n
