// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Cel.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for a basic animation CEL.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Layer/AnimatedLayer/Cel.h"
#include "Math/Math.h"
#include "Memory/Array.h"

ULIS_NAMESPACE_BEGIN

/////////////////////////////////////////////////////
/// FCelExtension
FCelFauxFixe::FCelFauxFixe(
      eCelFauxFixeMode iMode
    , uint32 iExposure
    , eCelExtensionExposureType iType
)
    : mMode( iMode )
    , mExposure( iExposure )
    , mType( iType )
{}

bool
FCelFauxFixe::operator==( const FCelFauxFixe& iOther ) const {
    return  mMode == iOther.mMode
        &&  mExposure == iOther.mExposure
        &&  mType == iOther.mType;
}

eCelFauxFixeMode
FCelFauxFixe::Mode() const {
    return  mMode;
}

uint32
FCelFauxFixe::Exposure() const {
    return  mExposure;
}

eCelExtensionExposureType
FCelFauxFixe::Type() const {
    return  mType;
}

void
FCelFauxFixe::SetMode( eCelFauxFixeMode iValue ) {
    mMode = iValue;
}

void
FCelFauxFixe::SetExposure( uint32 iValue ) {
    mExposure = FMath::Max( uint32( 1 ), iValue );
}

void
FCelFauxFixe::SetType( eCelExtensionExposureType iValue ) {
    mType = iValue;
}

/////////////////////////////////////////////////////
/// FCelExtension
FCelExtension::FCelExtension(
      eCelExtensionBehaviour iBehaviour
    , const FCelFauxFixe& iFauxFixe
)
    : mBehaviour( iBehaviour )
    , mFauxFixe( iFauxFixe )
{}

bool
FCelExtension::operator==( const FCelExtension& iOther ) const {
    return  mBehaviour == iOther.mBehaviour
        &&  mFauxFixe == iOther.mFauxFixe;
}

eCelExtensionBehaviour
FCelExtension::Behaviour() const {
    return  mBehaviour;
}

const FCelFauxFixe&
FCelExtension::FauxFixe() const {
    return  mFauxFixe;
}

void
FCelExtension::SetBehaviour( eCelExtensionBehaviour iValue ) {
    mBehaviour = iValue;
}

void
FCelExtension::SetFauxFixe( const FCelFauxFixe& iValue ) {
    mFauxFixe = iValue;
}

/////////////////////////////////////////////////////
/// FCelInfo
FCelInfo::FCelInfo(
      uint32 iExposure
    , const FCelExtension& iPreBehaviour
    , const FCelExtension& iPostBehaviour
)
    : mExposure( FMath::Max( uint32( 1 ), iExposure ) )
    , mPreBehaviour( iPreBehaviour )
    , mPostBehaviour( iPostBehaviour )
{}

bool
FCelInfo::operator==( const FCelInfo& iOther ) const {
    return  mExposure == iOther.mExposure
        &&  mPreBehaviour == iOther.mPreBehaviour
        &&  mPostBehaviour == iOther.mPostBehaviour;
}

uint32
FCelInfo::Exposure() const {
    return  mExposure;
}

FCelExtension&
FCelInfo::PreBehaviour() {
    return  mPreBehaviour;
}

const FCelExtension&
FCelInfo::PreBehaviour() const {
    return  mPreBehaviour;
}

FCelExtension&
FCelInfo::PostBehaviour() {
    return  mPostBehaviour;
}

const FCelExtension&
FCelInfo::PostBehaviour() const {
    return  mPostBehaviour;
}

void
FCelInfo::SetExposure( uint32 iValue ) {
    mExposure = FMath::Max( uint32( 1 ), iValue );
}

void
FCelInfo::SetPreBehaviour(FCelExtension iValue)
{
    mPreBehaviour = iValue;
}

void
FCelInfo::SetPostBehaviour(FCelExtension iValue)
{
    mPostBehaviour = iValue;
}

void
BlendCelInfos(const TArray<FCelInfo>& iCelInfos1, uint32 iFirstFrame1, const TArray<FCelInfo>& iCelInfos2, uint32 iFirstFrame2, TArray<FCelInfo>* oCelInfos, uint32* oFirstFrame)
{
    *oFirstFrame = FMath::Max(iFirstFrame1, iFirstFrame2);
    oCelInfos->Clear();

    uint32 index1 = 0;
    uint32 index2 = 0;
    
    uint32 frame1 = iFirstFrame1;
    uint32 frame2 = iFirstFrame2;
    uint32 frame3 = -1;

    while (frame1 != -1 || frame2 != -1)
    {
        if (index1 < iCelInfos1.Size() && index2 < iCelInfos2.Size()) 
        {
            //Push a new Cel
            oCelInfos->PushBack(FCelInfo());

            //Search for the CelInfos to take into account
            if (frame1 < frame2)
            {
                //If another has been previously processed, then define its exposure according to the new cel position
                if (oCelInfos->Size() > 0)
                    (*oCelInfos)[oCelInfos->Size() - 1].SetExposure(frame3 - frame1);

                //update navigation vars 
                frame3 = frame1;
                frame1 += iCelInfos1[index1].Exposure();
                index1++;
            }
            else
            {
                //If another has been previously processed, then define its exposure according to the new cel position
                if (oCelInfos->Size() > 0)
                    (*oCelInfos)[oCelInfos->Size() - 1].SetExposure(frame3 - frame2);

                //update navigation vars
                frame3 = frame2;

                if (frame1 == frame2)
                {
                    frame1 += iCelInfos1[index1].Exposure();
                    index1++;
                }

                frame2 += iCelInfos2[index2].Exposure();
                index2++;
            }
        }
        else if (index1 < iCelInfos1.Size())
        {
            
            if (frame2 == -1 || frame1 < frame2)
            {    
                //Push a new Cel
                oCelInfos->PushBack(FCelInfo());

                //If another has been previously processed, then define its exposure according to the new cel position
                if (oCelInfos->Size() > 0)
                    (*oCelInfos)[oCelInfos->Size() - 1].SetExposure(frame3 - frame1);

                //update navigation vars 
                frame3 = frame1;
                frame1 += iCelInfos1[index1].Exposure();
                index1++;
            }
            else
            {
                oCelInfos->PushBack(FCelInfo());
                
                //If another has been previously processed, then define its exposure according to the new cel position
                if (oCelInfos->Size() > 0)
                    (*oCelInfos)[oCelInfos->Size() - 1].SetExposure(frame3 - frame2);

                //update navigation vars 
                if (frame1 == frame2)
                {
                    frame1 += iCelInfos1[index1].Exposure();
                    index1++;
                }

                frame3 = frame2;
                frame2 = -1;
            }
        }
        else if (index2 < iCelInfos2.Size())
        {
            if (frame1 == -1 || frame2 < frame1)
            {    
                //Push a new Cel
                oCelInfos->PushBack(FCelInfo());

                //If another has been previously processed, then define its exposure according to the new cel position
                if (oCelInfos->Size() > 0)
                    (*oCelInfos)[oCelInfos->Size() - 1].SetExposure(frame3 - frame2);

                //update navigation vars 
                frame3 = frame2;
                frame2 += iCelInfos2[index2].Exposure();
                index2++;
            }
            else
            {
                oCelInfos->PushBack(FCelInfo());
                
                //If another has been previously processed, then define its exposure according to the new cel position
                if (oCelInfos->Size() > 0)
                    (*oCelInfos)[oCelInfos->Size() - 1].SetExposure(frame3 - frame1);

                //update navigation vars 
                if (frame1 == frame2)
                {
                    frame2 += iCelInfos2[index2].Exposure();
                    index2++;
                }

                frame3 = frame1;
                frame1 = -1;
            }
        }
        else
        {
            if (frame2 == -1)
            {
                 //If another has been previously processed, then define its exposure according to the new cel position
                if (oCelInfos->Size() > 0)
                    (*oCelInfos)[oCelInfos->Size() - 1].SetExposure(frame3 - frame1);

                frame3 = frame1;
                frame1 = -1;
            }
            else if (frame1 == -1)
            {
                //If another has been previously processed, then define its exposure according to the new cel position
                if (oCelInfos->Size() > 0)
                    (*oCelInfos)[oCelInfos->Size() - 1].SetExposure(frame3 - frame2);

                frame3 = frame2;
                frame2 = -1;
            }
            else
            {
                //Push a new Cel
                oCelInfos->PushBack(FCelInfo());

                //Search for the CelInfos to take into account
                if (frame1 < frame2)
                {
                    //If another has been previously processed, then define its exposure according to the new cel position
                    if (oCelInfos->Size() > 0)
                        (*oCelInfos)[oCelInfos->Size() - 1].SetExposure(frame3 - frame1);

                    //update navigation vars 
                    frame3 = frame1;
                    frame1 = -1;
                }
                else
                {
                    //If another has been previously processed, then define its exposure according to the new cel position
                    if (oCelInfos->Size() > 0)
                        (*oCelInfos)[oCelInfos->Size() - 1].SetExposure(frame3 - frame2);

                    //update navigation vars
                    frame3 = frame2;

                    if (frame1 == frame2)
                    {
                        frame1 = -1;
                    }

                    frame2 = -1;
                }
            }
        }
    }
}

ULIS_NAMESPACE_END

