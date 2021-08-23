set -e
cd "`dirname "$0"`"

## CD 3 steps up to root
cd ../../../../
cd ULIS/Tools/

## Make Generated dir for generated project if not exist
if [ ! -d EmbeddedDependencies ]; then 
	mkdir EmbeddedDependencies
fi
cd EmbeddedDependencies

git clone https://github.com/boostorg/preprocessor
git clone https://github.com/g-truc/glm
git clone https://github.com/Robot-Fromage/Coal
