set -e
cd "`dirname "$0"`"

if [ -f ULIS/Tools/Scripts/Mac/FullBuildUE4_MacXcodex64.sh ]; then
    sh ULIS/Tools/Scripts/Mac/FullBuildUE4_MacXcodex64.sh
fi