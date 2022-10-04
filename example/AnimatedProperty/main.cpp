// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         All.cpp
* @author       Clement Berthaud
* @brief        Example of all ctx features for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <chrono>
#include "Animation/DefaultValue.h"

using namespace ::ULIS;

int
main( int argc, char *argv[] ) {

    TBoundedAnimatedProperty<float> animatedFloat = TBoundedAnimatedProperty<float>( 1.0f, 1.f, 10.f );
    animatedFloat.AddOrReplaceKey( TKey<float>(60.2f, 2.1f, new TBezierInterpolation<float>()) );
    animatedFloat.AddOrReplaceKey( TKey<float>(55.1f, 7.2f, new TBezierInterpolation<float>()) );
    animatedFloat.AddOrReplaceKey( TKey<float>(38, 4.5f, new TBezierInterpolation<float>()) );
    animatedFloat.AddOrReplaceKey( TKey<float>(17, 3.5f, new TBezierInterpolation<float>()) );
    animatedFloat.AddOrReplaceKey( TKey<float>(44, 5.6f, new TBezierInterpolation<float>()) );
    animatedFloat.AddOrReplaceKey( TKey<float>(21, 1.5f, new TBezierInterpolation<float>()) );
    animatedFloat.AddOrReplaceKey( TKey<float>(44, 7.6f, new TBezierInterpolation<float>()) );

    animatedFloat.RemoveKeyAtFrame( 60.2f );

    TBezierInterpolation<float>* bezierInterp = dynamic_cast<TBezierInterpolation<float>*>( animatedFloat.GetKeys()[3].GetInterpolation() );
    bezierInterp->SetLeftTangent( FVec2F( -2.f, 0.5f ));
    bezierInterp->SetRightTangent( FVec2F( 1.5f, 1.f ));

    float value = animatedFloat.GetValueAtFrame( 48 );

    for (int i = 0; i < animatedFloat.GetKeys().Size(); i++)
    {
        std::cout << animatedFloat.GetKeys()[i].mFrame << ";" << animatedFloat.GetKeys()[i].mValue << std::endl;
    }

    std::cout << value << std::endl;

    return 0;
}

