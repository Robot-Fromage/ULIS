import os
from xml.dom import minidom

keys_begin = [
      "IHas"
    , "I"
    , "TAbstract"
    , "T"
    , "F"
]

keys_end = [
      "able"
]

colors = [
      "#5e5a3d"
    , "#5e3d5c"
    , "#423d5e"
    , "#3d515e"
    , "#3d5e40"
]

strokes = [
      "#403d29"
    , "#40293e"
    , "#2d2940"
    , "#3d515e"
    , "#29402b"
]

default_color = "#3887c7"

arrow_color = "#dcdcdc"
background_color = "#282828"
#background_color = "white"

def PostProcessSVG( iFile ):
    print( "Processing " + iFile )
    doc = minidom.parse( "./version_4.0.13/html/" + iFile )
    root = doc.documentElement

    # BACKGROUND
    polygon_tag = root.getElementsByTagName('polygon')
    if len( polygon_tag ) > 0:
        polygon = polygon_tag[0]
        polygon.setAttribute( 'fill', "#00000000" )

    #root.setAttribute( 'style', "background:" + background_color  + ";" )
    root.setAttribute( 'style', "background-image: url(graph_bg.png);" )

    # CLASSES AND COLOR
    for class_node in root.getElementsByTagName('g'):
        if( class_node.getAttribute('class') == "node" ):
            polygon_tag = class_node.getElementsByTagName('polygon')
            text_tag = class_node.getElementsByTagName('text')
            className = "None"
            polygon = None
            if len( polygon_tag ) > 0:
                polygon = polygon_tag[0]

            if len( text_tag ) > 0:
                className = text_tag[0].firstChild.data

            for it in text_tag:
                it.setAttribute( 'fill', "white" )

            count = 0
            polygon.setAttribute( 'fill', default_color )
            for it in keys_begin:
                if className.startswith( it ):
                    polygon.setAttribute( 'fill', colors[count] )
                    polygon.setAttribute( 'stroke', strokes[count] )
                    break
                count+=1

            try:
                pass#polygon.removeAttribute( 'stroke' )
            except:
                pass
            #print( className )
            #print( polygon.getAttribute( 'fill' ) )

    # EDGES ARROWS
    for class_node in root.getElementsByTagName('g'):
        if( class_node.getAttribute('class') == "edge" ):
            path_tag = class_node.getElementsByTagName('path')
            polygon_tag = class_node.getElementsByTagName('polygon')
            if len( polygon_tag ) > 0:
                polygon = polygon_tag[0]
                polygon.setAttribute( 'fill', arrow_color )
                polygon.setAttribute( 'stroke', arrow_color )

            if len( path_tag ) > 0:
                path = path_tag[0]
                path.setAttribute( 'stroke', arrow_color )

    # NAVIGATOR
    for gnode in root.getElementsByTagName('g'):
        if( gnode.getAttribute('id') == "navigator" ):
            old = root.removeChild( gnode )
            old.unlink()

    # SAVE
    text_file = open( "./version_4.0.13/html/" + iFile, "w" )
    n = text_file.write(  doc.toxml() )
    text_file.close()

for file in os.listdir( "./version_4.0.13/html/" ):
    if file.endswith( ".svg" ) and file.startswith( "class_" ):
        PostProcessSVG( file )

