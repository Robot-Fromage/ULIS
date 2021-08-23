set -e
cd "`dirname "$0"`"

if [ -f ULIS/Tools/Scripts/Linux/Generate_SublimeText_Project_GNU_GCC.sh ]; then
    sh ULIS/Tools/Scripts/Linux/Generate_SublimeText_Project_GNU_GCC.sh
fi