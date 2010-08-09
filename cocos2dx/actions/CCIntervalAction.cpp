/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCIntervalAction.h"
#include "CCSprite.h"
#include "CCNode.h"
#include "support/CGPointExtension.h"

#include <float.h>

namespace cocos2d {

//
// IntervalAction
//
CCIntervalAction* CCIntervalAction::actionWithDuration(ccTime d)
{
	CCIntervalAction *pAction = new CCIntervalAction();
	pAction->initWithDuration(d);
	pAction->autorelease();

	return pAction;
}

CCIntervalAction* CCIntervalAction::initWithDuration(cocos2d::ccTime d)
{
	m_fDuration = d;

	// prevent division by 0
	// This comparison could be in step:, but it might decrease the performance
	// by 3% in heavy based action games.
	if (m_fDuration == 0)
	{
		m_fDuration = FLT_EPSILON;
	}

	m_elapsed = 0;
	m_bFirstTick = true;

	return this;
}

NSObject* CCIntervalAction::copyWithZone(NSZone *pZone)
{
	NSZone* pNewZone = NULL;
	CCIntervalAction* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = static_cast<CCIntervalAction*>(pZone->m_pCopyObject);
	}
	else
	{
		// action's base class , must be called using __super::copyWithZone(), after overriding from derived class
		assert(0);  

		pCopy = new CCIntervalAction();
		pZone = pNewZone = new NSZone(pCopy);
	}

	
	__super::copyWithZone(pZone);

	CCX_SAFE_DELETE(pNewZone);

	pCopy->initWithDuration(m_fDuration);

	return pCopy;
}

bool CCIntervalAction::isDone(void)
{
	return m_elapsed >= m_fDuration;
}

void CCIntervalAction::step(ccTime dt)
{
	if (m_bFirstTick)
	{
		m_bFirstTick = false;
		m_elapsed = 0;
	}
	else
	{
		m_elapsed += dt;
	}

	update(min(1, m_elapsed/m_fDuration));
}

void CCIntervalAction::startWithTarget(NSObject *pTarget)
{
	__super::startWithTarget(pTarget);
	m_elapsed = 0.0f;
	m_bFirstTick = true;
}

CCIntervalAction* CCIntervalAction::reverse(void)
{
	/*
	 NSException* myException = [NSException
								exceptionWithName:@"ReverseActionNotImplemented"
								reason:@"Reverse Action not implemented"
								userInfo:nil];
	@throw myException;	
	*/
	return NULL;
}

//
// Sequence
//
CCSequence* CCSequence::actionOneTwo(cocos2d::CCFiniteTimeAction *pActionOne, cocos2d::CCFiniteTimeAction *pActionTwo)
{
	CCSequence *pSequence = new CCSequence();
	pSequence->initOneTwo(pActionOne, pActionTwo);
	pSequence->autorelease();

	return pSequence;
}

CCFiniteTimeAction* CCSequence::actions(cocos2d::CCFiniteTimeAction *pAction1, ...)
{
	va_list params;
	va_start(params, pAction1);

	CCFiniteTimeAction *pNow;
	CCFiniteTimeAction *pPrev = pAction1;

	while (pAction1)
	{
		pNow = va_arg(params, CCFiniteTimeAction*);
		if (pNow)
		{
			pPrev = actionOneTwo(pPrev, pNow);
		}
		else
		{
			break;
		}
	}

	va_end(params);
	return pPrev;
}

CCSequence* CCSequence::initOneTwo(cocos2d::CCFiniteTimeAction *pActionOne, cocos2d::CCFiniteTimeAction *pActionTwo)
{
	assert(pActionOne != NULL);
	assert(pActionTwo != NULL);

	ccTime d = pActionOne->getDuration() + pActionTwo->getDuration();
	__super::initWithDuration(d);

	m_pActions[0] = pActionOne;
	pActionOne->retain();

	m_pActions[1] = pActionTwo;
	pActionTwo->retain();

	return this;
}

NSObject* CCSequence::copyWithZone(NSZone *pZone)
{
	NSZone* pNewZone = NULL;
	CCSequence* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = dynamic_cast<CCSequence*>(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCSequence();
		pZone = pNewZone = new NSZone(pCopy);
	}

	__super::copyWithZone(pZone);

	pCopy->initOneTwo(static_cast<CCFiniteTimeAction*>(m_pActions[0]->copy()->autorelease()), 
				static_cast<CCFiniteTimeAction*>(m_pActions[1]->copy()->autorelease()));

	CCX_SAFE_DELETE(pNewZone);
	return pCopy;
}

CCSequence::~CCSequence(void)
{
	m_pActions[0]->release();
	m_pActions[1]->release();
}

void CCSequence::startWithTarget(cocos2d::NSObject *pTarget)
{
	__super::startWithTarget(pTarget);
	m_split = m_pActions[0]->getDuration() / m_fDuration;
	m_last = -1;
}

void CCSequence::stop(void)
{
	m_pActions[0]->stop();
	m_pActions[1]->stop();
	__super::stop();
}

void CCSequence::update(cocos2d::ccTime time)
{
	int found = 0;
	ccTime new_t = 0.0f;

	if (time >= m_split)
	{
		found = 1;
		if (m_split == 1)
		{
			new_t = 1;
		}
		else
		{
			new_t = (time - m_split) / (1 - m_split);
		}
	}
	else
	{
		found = 0;
		if (m_split != 0)
		{
			new_t = time / m_split;
		}
		else
		{
			new_t = 1;
		}
	}

	if (m_last == -1 && found == 1)
	{
		m_pActions[0]->startWithTarget(m_pTarget);
		m_pActions[0]->update(1.0f);
		m_pActions[0]->stop();
	}

	if (m_last != found)
	{
		if (m_last != -1)
		{
			m_pActions[m_last]->update(1.0f);
			m_pActions[m_last]->stop();
		}

		m_pActions[found]->startWithTarget(m_pTarget);
	}

	m_pActions[found]->update(new_t);
	m_last = found;
}

CCIntervalAction* CCSequence::reverse(void)
{
	return CCSequence::actionOneTwo(m_pActions[1]->reverse(), m_pActions[0]->reverse());
}

//
// Repeat
//
CCRepeat* CCRepeat::actionWithAction(cocos2d::CCFiniteTimeAction *pAction, unsigned int times)
{
	CCRepeat* pRepeat = new CCRepeat();
	pRepeat->initWithAction(pAction, times);
	pRepeat->autorelease();

	return pRepeat;
}

CCRepeat* CCRepeat::initWithAction(cocos2d::CCFiniteTimeAction *pAction, unsigned int times)
{
	ccTime d = pAction->getDuration() * times;

	if (__super::initWithDuration(d))
	{
        m_uTimes = times;
		m_pOther = pAction;
		pAction->retain();

		m_uTotal = 0;
	}

	return this;
}

NSObject* CCRepeat::copyWithZone(cocos2d::NSZone *pZone)
{
	
	NSZone* pNewZone = NULL;
	CCRepeat* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = dynamic_cast<CCRepeat*>(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCRepeat();
		pZone = pNewZone = new NSZone(pCopy);
	}

	__super::copyWithZone(pZone);

	pCopy->initWithAction(static_cast<CCFiniteTimeAction*>(m_pOther->copy()->autorelease()), m_uTimes);

	CCX_SAFE_DELETE(pNewZone);
	return pCopy;
}

CCRepeat::~CCRepeat(void)
{
	m_pOther->release();
}

void CCRepeat::startWithTarget(cocos2d::NSObject *pTarget)
{
	m_uTotal = 0;
	__super::startWithTarget(pTarget);
	m_pOther->startWithTarget(pTarget);
}

void CCRepeat::stop(void)
{
	m_pOther->stop();
	__super::stop();
}

// issue #80. Instead of hooking step:, hook update: since it can be called by any 
// container action like Repeat, Sequence, AccelDeccel, etc..
void CCRepeat::update(cocos2d::ccTime time)
{
	ccTime t = time * m_uTimes;
	if (t > m_uTotal + 1)
	{
		m_pOther->update(1.0f);
		m_uTotal++;
		m_pOther->stop();
		m_pOther->startWithTarget(m_pTarget);

		// repeat is over?
		if (m_uTotal == m_uTimes)
		{
			// so, set it in the original position
			m_pOther->update(0);
		}
		else
		{
			// no ? start next repeat with the right update
			// to prevent jerk (issue #390)
			m_pOther->update(t - m_uTotal);
		}
	}
	else
	{
		float r = fmodf(t, 1.0f);

		// fix last repeat position
		// else it could be 0.
		if (time == 1.0f)
		{
			r = 1.0f;
			m_uTotal++; // this is the added line
		}

		m_pOther->update(min(r, 1));
	}
}

bool CCRepeat::isDone(void)
{
	return m_uTotal == m_uTimes;
}

CCIntervalAction* CCRepeat::reverse(void)
{
	return CCRepeat::actionWithAction(m_pOther->reverse(), m_uTimes);
}

//
// Spawn
//
CCFiniteTimeAction* CCSpawn::actions(cocos2d::CCFiniteTimeAction *pAction1, ...)
{
	va_list params;
	va_start(params, pAction1);

	CCFiniteTimeAction *pNow;
	CCFiniteTimeAction *pPrev = pAction1;

	while (pAction1)
	{
		pNow = va_arg(params, CCFiniteTimeAction*);
		if (pNow)
		{
			pPrev = actionOneTwo(pPrev, pNow);
		}
		else
		{
			break;
		}
	}

	va_end(params);
	return pPrev;
}

CCSpawn* CCSpawn::actionOneTwo(cocos2d::CCFiniteTimeAction *pAction1, cocos2d::CCFiniteTimeAction *pAction2)
{
	CCSpawn *pSpawn = new CCSpawn();
	pSpawn->initOneTwo(pAction1, pAction2);
	pSpawn->autorelease();

	return pSpawn;
}

CCSpawn* CCSpawn:: initOneTwo(CCFiniteTimeAction *pAction1, CCFiniteTimeAction *pAction2)
{
	assert(pAction1 != NULL);
	assert(pAction2 != NULL);

	ccTime d1 = pAction1->getDuration();
	ccTime d2 = pAction2->getDuration();

	// __super::initWithDuration(fmaxf(d1, d2));
	float maxd = (d1 >= d2 || _isnan(d2)) ? d1 : d2;
	__super::initWithDuration(maxd);

    m_pOne = pAction1;
	m_pTwo = pAction2;

	if (d1 > d2)
	{
		m_pTwo = CCSequence::actionOneTwo(pAction1, CCDelayTime::actionWithDuration(d1 - d2));
	} else
	if (d1 < d2)
	{
		m_pOne = CCSequence::actionOneTwo(pAction1, CCDelayTime::actionWithDuration(d2 - d1));
	}

	pAction1->retain();
	pAction2->retain();
	return this;
}

NSObject* CCSpawn::copyWithZone(cocos2d::NSZone *pZone)
{
	NSZone* pNewZone = NULL;
	CCSpawn* pCopy = NULL;

	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = dynamic_cast<CCSpawn*>(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCSpawn();
		pZone = pNewZone = new NSZone(pCopy);
	}

	__super::copyWithZone(pZone);

	pCopy->initOneTwo(	dynamic_cast<CCFiniteTimeAction*>(m_pOne->copy()->autorelease()), 
					dynamic_cast<CCFiniteTimeAction*>(m_pTwo->copy()->autorelease()));

	CCX_SAFE_DELETE(pNewZone);
	return pCopy;
}

CCSpawn::~CCSpawn(void)
{
	m_pOne->release();
	m_pTwo->release();
}

void CCSpawn::startWithTarget(cocos2d::NSObject *pTarget)
{
	__super::startWithTarget(pTarget);
	m_pOne->startWithTarget(pTarget);
	m_pTwo->startWithTarget(pTarget);
}

void CCSpawn::stop(void)
{
	m_pOne->stop();
	m_pTwo->stop();
	__super::stop();
}

void CCSpawn::update(cocos2d::ccTime time)
{
	m_pOne->update(time);
	m_pTwo->update(time);
}

CCIntervalAction* CCSpawn::reverse(void)
{
	return CCSpawn::actionOneTwo(m_pOne->reverse(), m_pTwo->reverse());
}

//
// RotateTo
//
CCRotateTo* CCRotateTo::actionWithDuration(cocos2d::ccTime duration, float fDeltaAngle)
{
	CCRotateTo* pRotateTo = new CCRotateTo();
	pRotateTo->initWithDuration(duration, fDeltaAngle);
	pRotateTo->autorelease();

	return pRotateTo;
}

CCRotateTo* CCRotateTo::initWithDuration(cocos2d::ccTime duration, float fDeltaAngle)
{
	if (__super::initWithDuration(duration))
	{
		m_fDstAngle = fDeltaAngle;
		return this;
	}

	return NULL;
}

NSObject* CCRotateTo::copyWithZone(cocos2d::NSZone *pZone)
{
	NSZone* pNewZone = NULL;
	CCRotateTo* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject)
	{
		//in case of being called at sub class
		pCopy = dynamic_cast<CCRotateTo*>(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCRotateTo();
		pZone = pNewZone = new NSZone(pCopy);
	}

	__super::copyWithZone(pZone);

	pCopy->initWithDuration(m_fDuration, m_fDstAngle);

	//Action *copy = [[[self class] allocWithZone: zone] initWithDuration:[self duration] angle: angle];
	CCX_SAFE_DELETE(pNewZone);
	return pCopy;
}

void CCRotateTo::startWithTarget(cocos2d::NSObject *pTarget)
{
	__super::startWithTarget(pTarget);

	m_fStartAngle = (dynamic_cast<CCNode*>(pTarget))->getRotation();

	if (m_fStartAngle > 0)
	{
		m_fStartAngle = fmodf(m_fStartAngle, 360.0f);
	}
	else
	{
		m_fStartAngle = fmodf(m_fStartAngle, -360.0f);
	}

	m_fDiffAngle = m_fDstAngle - m_fStartAngle;
	if (m_fDiffAngle > 180)
	{
		m_fDiffAngle -= 360;
	}

	if (m_fDiffAngle < -180)
	{
		m_fDiffAngle += 360;
	}
}

void CCRotateTo::update(cocos2d::ccTime time)
{
	(dynamic_cast<CCNode*>(m_pTarget))->setRotation(m_fStartAngle + m_fDiffAngle * time);
}

//
// RotateBy
//
CCRotateBy* CCRotateBy::actionWithDuration(cocos2d::ccTime duration, float fDeltaAngle)
{
	CCRotateBy *pRotateBy = new CCRotateBy();
	pRotateBy->initWithDuration(duration, fDeltaAngle);
	pRotateBy->autorelease();

	return pRotateBy;
}

CCRotateBy* CCRotateBy::initWithDuration(cocos2d::ccTime duration, float fDeltaAngle)
{
	if (__super::initWithDuration(duration))
	{
		m_fAngle = fDeltaAngle;
		return this;
	}

	return NULL;
}

NSObject* CCRotateBy::copyWithZone(cocos2d::NSZone *pZone)
{
	NSZone* pNewZone = NULL;
	CCRotateBy* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = dynamic_cast<CCRotateBy*>(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCRotateBy();
		pZone = pNewZone = new NSZone(pCopy);
	}

	__super::copyWithZone(pZone);

	pCopy->initWithDuration(m_fDuration, m_fAngle);

	CCX_SAFE_DELETE(pNewZone);
	return pCopy;
}

void CCRotateBy::startWithTarget(cocos2d::NSObject *pTarget)
{
	__super::startWithTarget(pTarget);
	m_fStartAngle = dynamic_cast<CCNode*>(pTarget)->getRotation();
}

void CCRotateBy::update(cocos2d::ccTime time)
{
	// XXX: shall I add % 360
	dynamic_cast<CCNode*>(m_pTarget)->setRotation(m_fStartAngle + m_fAngle * time);
}

CCIntervalAction* CCRotateBy::reverse(void)
{
	return CCRotateBy::actionWithDuration(m_fDuration, -m_fAngle);
}

//
// MoveTo
//
CCMoveTo* CCMoveTo::actionWithDuration(cocos2d::ccTime duration, cocos2d::CGPoint position)
{
	CCMoveTo *pMoveTo = new CCMoveTo();
	pMoveTo->initWithDuration(duration, position);
	pMoveTo->autorelease();

	return pMoveTo;
}

CCMoveTo* CCMoveTo::initWithDuration(cocos2d::ccTime duration, cocos2d::CGPoint position)
{
	if (__super::initWithDuration(duration))
	{
		m_endPosition = position;
		return this;
	}

	return NULL;
}

NSObject* CCMoveTo::copyWithZone(cocos2d::NSZone *pZone)
{
	NSZone* pNewZone = NULL;
	CCMoveTo* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = dynamic_cast<CCMoveTo*>(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCMoveTo();
		pZone = pNewZone = new NSZone(pCopy);
	}

	__super::copyWithZone(pZone);

	pCopy->initWithDuration(m_fDuration, m_endPosition);

	CCX_SAFE_DELETE(pNewZone);
	return pCopy;
}

void CCMoveTo::startWithTarget(NSObject *pTarget)
{
	__super::startWithTarget(pTarget);
	m_startPosition = dynamic_cast<CCNode*>(pTarget)->getPosition();
	m_delta = ccpSub(m_endPosition, m_startPosition);
}

void CCMoveTo::update(cocos2d::ccTime time)
{
	dynamic_cast<CCNode*>(m_pTarget)->setPosition(ccp(m_startPosition.x + m_delta.x * time,
		m_startPosition.y + m_delta.y * time));
}

//
// MoveBy
//
CCMoveBy* CCMoveBy::actionWithDuration(cocos2d::ccTime duration, cocos2d::CGPoint position)
{
	CCMoveBy *pMoveBy = new CCMoveBy();
	pMoveBy->initWithDuration(duration, position);
	pMoveBy->autorelease();

	return pMoveBy;
}

CCMoveBy* CCMoveBy::initWithDuration(cocos2d::ccTime duration, cocos2d::CGPoint position)
{
	if (CCIntervalAction::initWithDuration(duration))
	{
		m_delta = position;
		return this;
	}

	return NULL;
}

NSObject* CCMoveBy::copyWithZone(cocos2d::NSZone *pZone)
{
	NSZone* pNewZone = NULL;
	CCMoveBy* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = dynamic_cast<CCMoveBy*>(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCMoveBy();
		pZone = pNewZone = new NSZone(pCopy);
	}

	__super::copyWithZone(pZone);

	pCopy->initWithDuration(m_fDuration, m_delta);
	
	CCX_SAFE_DELETE(pNewZone);
	return pCopy;
}

void CCMoveBy::startWithTarget(cocos2d::NSObject *pTarget)
{
	CGPoint dTmp = m_delta;
	__super::startWithTarget(pTarget);
	m_delta = dTmp;
}

CCIntervalAction* CCMoveBy::reverse(void)
{
	return CCMoveBy::actionWithDuration(m_fDuration, ccp(-m_delta.x, -m_delta.y));
}

//
// JumpBy
//
CCJumpBy* CCJumpBy::actionWithDuration(cocos2d::ccTime duration, cocos2d::CGPoint position, cocos2d::ccTime height, int jumps)
{
	CCJumpBy *pJumpBy = new CCJumpBy();
	pJumpBy->initWithDuration(duration, position, height, jumps);
	pJumpBy->autorelease();

	return pJumpBy;
}

CCJumpBy* CCJumpBy::initWithDuration(cocos2d::ccTime duration, cocos2d::CGPoint position, cocos2d::ccTime height, int jumps)
{
	if (__super::initWithDuration(duration))
	{
        m_delta = position;
		m_height = m_height;
		m_nJumps = jumps;

		return this;
	}

	return NULL;
}

NSObject* CCJumpBy::copyWithZone(cocos2d::NSZone *pZone)
{
	NSZone* pNewZone = NULL;
	CCJumpBy* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = dynamic_cast<CCJumpBy*>(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCJumpBy();
		pZone = pNewZone = new NSZone(pCopy);
	}

	__super::copyWithZone(pZone);

	pCopy->initWithDuration(m_fDuration, m_delta, m_height, m_nJumps);
	
	CCX_SAFE_DELETE(pNewZone);
	return pCopy;
}

void CCJumpBy::startWithTarget(cocos2d::NSObject *pTarget)
{
	__super::startWithTarget(pTarget);
	m_startPosition = dynamic_cast<CCNode*>(pTarget)->getPosition();
}

void CCJumpBy::update(cocos2d::ccTime time)
{
	// parabolic jump (since v0.8.2)
	ccTime frac = fmodf(time * m_nJumps, 1.0f);
	ccTime y = m_height * 4 * frac * (1 - frac);
	y += m_delta.y * time;
	ccTime x = m_delta.x * time;
	dynamic_cast<CCNode*>(m_pTarget)->setPosition(ccp(m_startPosition.x + x, m_startPosition.y + y));
}

CCIntervalAction* CCJumpBy::reverse(void)
{
	return CCJumpBy::actionWithDuration(m_fDuration, ccp(-m_delta.x, -m_delta.y),
		m_height, m_nJumps);
}

//
// JumpTo
//
void CCJumpTo::startWithTarget(cocos2d::NSObject *pTarget)
{
	__super::startWithTarget(pTarget);
	m_delta = ccp(m_delta.x - m_startPosition.x, m_delta.y - m_startPosition.y);
}

// Bezier cubic formula:
//	((1 - t) + t)3 = 1 
// Expands to�� 
//   (1 - t)3 + 3t(1-t)2 + 3t2(1 - t) + t3 = 1 
static inline float bezierat( float a, float b, float c, float d, ccTime t )
{
	return (powf(1-t,3) * a + 
			3*t*(powf(1-t,2))*b + 
			3*powf(t,2)*(1-t)*c +
			powf(t,3)*d );
}

//
// BezierBy
//
CCBezierBy* CCBezierBy::actionWithDuration(cocos2d::ccTime t, cocos2d::ccBezierConfig c)
{
	CCBezierBy *pBezierBy = new CCBezierBy();
	pBezierBy->initWithDuration(t, c);
	pBezierBy->autorelease();

	return pBezierBy;
}

CCBezierBy* CCBezierBy::initWithDuration(cocos2d::ccTime t, cocos2d::ccBezierConfig c)
{
	if (__super::initWithDuration(t))
	{
        m_sConfig = c;
		return this;
	}

	return NULL;
}

void CCBezierBy::startWithTarget(cocos2d::NSObject *pTarget)
{
	__super::startWithTarget(pTarget);
	m_startPosition = dynamic_cast<CCNode*>(pTarget)->getPosition();
}

NSObject* CCBezierBy::copyWithZone(cocos2d::NSZone *pZone)
{
	NSZone* pNewZone = NULL;
	CCBezierBy* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = dynamic_cast<CCBezierBy*>(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCBezierBy();
		pZone = pNewZone = new NSZone(pCopy);
	}

	__super::copyWithZone(pZone);

	pCopy->initWithDuration(m_fDuration, m_sConfig);
    
	CCX_SAFE_DELETE(pNewZone);
	return pCopy;
}

void CCBezierBy::update(cocos2d::ccTime time)
{
	float xa = 0;
	float xb = m_sConfig.controlPoint_1.x;
	float xc = m_sConfig.controlPoint_2.x;
	float xd = m_sConfig.endPosition.x;

	float ya = 0;
	float yb = m_sConfig.controlPoint_1.y;
	float yc = m_sConfig.controlPoint_2.y;
	float yd = m_sConfig.endPosition.y;

	float x = bezierat(xa, xb, xc, xd, time);
	float y = bezierat(ya, yb, yc, yd, time);
	dynamic_cast<CCNode*>(m_pTarget)->setPosition(ccpAdd(m_startPosition, ccp(x, y)));
}

CCIntervalAction* CCBezierBy::reverse(void)
{
	ccBezierConfig r;

	r.endPosition = ccpNeg(m_sConfig.endPosition);
	r.controlPoint_1 = ccpAdd(m_sConfig.controlPoint_2, ccpNeg(m_sConfig.endPosition));
	r.controlPoint_2 = ccpAdd(m_sConfig.controlPoint_1, ccpNeg(m_sConfig.endPosition));

	CCBezierBy *pAction = CCBezierBy::actionWithDuration(m_fDuration, r);
	return pAction;
}

//
// BezierTo
//
void CCBezierTo::startWithTarget(cocos2d::NSObject *pTarget)
{
	__super::startWithTarget(pTarget);
	m_sConfig.controlPoint_1 = ccpSub(m_sConfig.controlPoint_1, m_startPosition);
	m_sConfig.controlPoint_2 = ccpSub(m_sConfig.controlPoint_2, m_startPosition);
	m_sConfig.endPosition = ccpSub(m_sConfig.endPosition, m_startPosition);
}

//
// ScaleTo
//
CCScaleTo* CCScaleTo::actionWithDuration(cocos2d::ccTime duration, float s)
{
	CCScaleTo *pScaleTo = new CCScaleTo();
	pScaleTo->initWithDuration(duration, s);
	pScaleTo->autorelease();

	return pScaleTo;
}

CCScaleTo* CCScaleTo::initWithDuration(cocos2d::ccTime duration, float s)
{
	if (__super::initWithDuration(duration))
	{
        m_fEndScaleX = s;
		m_fEndScaleY = s;

		return this;
	}

	return NULL;
}

CCScaleTo* CCScaleTo::actionWithDuration(cocos2d::ccTime duration, float sx, float sy)
{
	CCScaleTo *pScaleTo = new CCScaleTo();
	pScaleTo->initWithDuration(duration, sx, sy);
	pScaleTo->autorelease();

	return pScaleTo;
}

CCScaleTo* CCScaleTo::initWithDuration(cocos2d::ccTime duration, float sx, float sy)
{
	if (__super::initWithDuration(duration))
	{
		m_fEndScaleX = sx;
		m_fEndScaleY = sy;

		return this;
	}

	return NULL;
}

NSObject* CCScaleTo::copyWithZone(cocos2d::NSZone *pZone)
{
	NSZone* pNewZone = NULL;
	CCScaleTo* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = dynamic_cast<CCScaleTo*>(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCScaleTo();
		pZone = pNewZone = new NSZone(pCopy);
	}

	__super::copyWithZone(pZone);


	pCopy->initWithDuration(m_fDuration, m_fEndScaleX, m_fEndScaleY);

	CCX_SAFE_DELETE(pNewZone);
	return pCopy;
}

void CCScaleTo::startWithTarget(cocos2d::NSObject *pTarget)
{
	__super::startWithTarget(pTarget);
	m_fStartScaleX = dynamic_cast<CCNode*>(pTarget)->getScaleX();
	m_fStartScaleY = dynamic_cast<CCNode*>(pTarget)->getScaleY();
    m_fDeltaX = m_fEndScaleX - m_fStartScaleX;
	m_fDeltaY = m_fEndScaleY - m_fStartScaleY;
}

void CCScaleTo::update(cocos2d::ccTime time)
{
	dynamic_cast<CCNode*>(m_pTarget)->setScaleX(m_fStartScaleX + m_fDeltaX * time);
	dynamic_cast<CCNode*>(m_pTarget)->setScaleY(m_fStartScaleY + m_fDeltaY * time);
}

//
// ScaleBy
//
void CCScaleBy::startWithTarget(cocos2d::NSObject *pTarget)
{
	__super::startWithTarget(pTarget);
	m_fDeltaX = m_fStartScaleX * m_fEndScaleX - m_fStartScaleX;
	m_fDeltaY = m_fStartScaleY * m_fEndScaleY - m_fStartScaleY;
}

CCIntervalAction* CCScaleBy::reverse(void)
{
	return CCScaleBy::actionWithDuration(m_fDuration, 1 / m_fEndScaleX, 1 / m_fEndScaleY);
}

//
// Blink
//
CCBlink* CCBlink::actionWithDuration(cocos2d::ccTime duration, unsigned int uBlinks)
{
	CCBlink *pBlink = new CCBlink();
	pBlink->initWithDuration(duration, uBlinks);
	pBlink->autorelease();

	return pBlink;
}

CCBlink* CCBlink::initWithDuration(cocos2d::ccTime duration, unsigned int uBlinks)
{
	if (__super::initWithDuration(duration))
	{
        m_nTimes = uBlinks;
		return this;
	}

	return NULL;
}

NSObject* CCBlink::copyWithZone(cocos2d::NSZone *pZone)
{
	NSZone* pNewZone = NULL;
	CCBlink* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = dynamic_cast<CCBlink*>(pZone->m_pCopyObject);

	}
	else
	{
		pCopy = new CCBlink();
		pZone = pNewZone = new NSZone(pCopy);
	}

	__super::copyWithZone(pZone);

	pCopy->initWithDuration(m_fDuration, (unsigned int)m_nTimes);
	
	CCX_SAFE_DELETE(pNewZone);
	return pCopy;
}

void CCBlink::update(cocos2d::ccTime time)
{
	ccTime slice = 1.0f / m_nTimes;
	ccTime m = fmodf(time, slice);
	dynamic_cast<CCNode*>(m_pTarget)->setIsVisible(m > slice / 2 ? true : false);
}

CCIntervalAction* CCBlink::reverse(void)
{
	// return 'self'
	return CCBlink::actionWithDuration(m_fDuration, m_nTimes);
}

//
// FadeIn
//
void CCFadeIn::update(cocos2d::ccTime time)
{
	dynamic_cast<CCRGBAProtocol*>(m_pTarget)->setOpacity((GLubyte)(255 * time));
}

CCIntervalAction* CCFadeIn::reverse(void)
{
	return CCFadeOut::actionWithDuration(m_fDuration);
}

//
// FadeOut
//
void CCFadeOut::update(cocos2d::ccTime time)
{
	dynamic_cast<CCRGBAProtocol*>(m_pTarget)->setOpacity(GLubyte(255 * (1 - time)));
}

CCIntervalAction* CCFadeOut::reverse(void)
{
	return CCFadeIn::actionWithDuration(m_fDuration);
}

//
// FadeTo
//
CCFadeTo* CCFadeTo::actionWithDuration(cocos2d::ccTime duration, GLubyte opacity)
{
	CCFadeTo *pFadeTo = new CCFadeTo();
	pFadeTo->initWithDuration(duration, opacity);
	pFadeTo->autorelease();

	 return pFadeTo;
}

CCFadeTo* CCFadeTo::initWithDuration(cocos2d::ccTime duration, GLubyte opacity)
{
	if (__super::initWithDuration(duration))
	{
        m_toOpacity = opacity;
		return this;
	}

	return NULL;
}

NSObject* CCFadeTo::copyWithZone(cocos2d::NSZone *pZone)
{
	NSZone* pNewZone = NULL;
	CCFadeTo* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = dynamic_cast<CCFadeTo*>(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCFadeTo();
		pZone = pNewZone = new NSZone(pCopy);
	}

	__super::copyWithZone(pZone);

	pCopy->initWithDuration(m_fDuration, m_toOpacity);
	
	CCX_SAFE_DELETE(pNewZone);
	return pCopy;
}

void CCFadeTo::startWithTarget(cocos2d::NSObject *pTarget)
{
	__super::startWithTarget(pTarget);

	m_fromOpacity = dynamic_cast<CCRGBAProtocol*>(pTarget)->getOpacity();
}

void CCFadeTo::update(cocos2d::ccTime time)
{
	dynamic_cast<CCRGBAProtocol*>(m_pTarget)->setOpacity((GLubyte)(m_fromOpacity + (m_toOpacity - m_fromOpacity) * time));
}

//
// TintTo
//
CCTintTo* CCTintTo::actionWithDuration(cocos2d::ccTime duration, GLubyte red, GLubyte green, GLubyte blue)
{
	CCTintTo *pTintTo = new CCTintTo();
	pTintTo->initWithDuration(duration, red, green, blue);
	pTintTo->autorelease();

	return pTintTo;
}

CCTintTo* CCTintTo::initWithDuration(cocos2d::ccTime duration, GLubyte red, GLubyte green, GLubyte blue)
{
	if (__super::initWithDuration(duration))
	{
        m_to = ccc3(red, green, blue);
		return this;
	}

	return NULL;
}

NSObject* CCTintTo::copyWithZone(cocos2d::NSZone *pZone)
{
	NSZone* pNewZone = NULL;
	CCTintTo* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = dynamic_cast<CCTintTo*>(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCTintTo();
		pZone = pNewZone = new NSZone(pCopy);
	}

	__super::copyWithZone(pZone);

	pCopy->initWithDuration(m_fDuration, m_to.r, m_to.g, m_to.b);
	
	CCX_SAFE_DELETE(pNewZone);
	return pCopy;
}

void CCTintTo::startWithTarget(cocos2d::NSObject *pTarget)
{
	__super::startWithTarget(pTarget);

	m_from = dynamic_cast<CCRGBAProtocol*>(pTarget)->getColor();
}

void CCTintTo::update(cocos2d::ccTime time)
{
	CCRGBAProtocol *pTn = dynamic_cast<CCRGBAProtocol*>(m_pTarget);
	pTn->setColor(ccc3(GLubyte(m_from.r + (m_to.r - m_from.r) * time), 
		              (GLbyte)(m_from.g + (m_to.g - m_from.g) * time),
		              (GLbyte)(m_from.b + (m_to.b - m_from.b) * time)));
}

//
// TintBy
//
CCTintBy* CCTintBy::actionWithDuration(cocos2d::ccTime duration, GLshort deltaRed, GLshort deltaGreen, GLshort deltaBlue)
{
	CCTintBy *pTintBy = new CCTintBy();
	pTintBy->initWithDuration(duration, deltaRed, deltaGreen, deltaBlue);
	pTintBy->autorelease();

	return pTintBy;
}

CCTintBy* CCTintBy::initWithDuration(cocos2d::ccTime duration, GLshort deltaRed, GLshort deltaGreen, GLshort deltaBlue)
{
	if (__super::initWithDuration(duration))
	{
        m_deltaR = deltaRed;
		m_deltaG = deltaGreen;
		m_deltaB = deltaBlue;

		return this;
	}

	return NULL;
}

NSObject* CCTintBy::copyWithZone(cocos2d::NSZone *pZone)
{
	NSZone* pNewZone = NULL;
	CCTintBy* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = dynamic_cast<CCTintBy*>(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCTintBy();
		pZone = pNewZone = new NSZone(pCopy);
	}

	__super::copyWithZone(pZone);

	pCopy->initWithDuration(m_fDuration, (GLubyte)m_deltaR, (GLubyte)m_deltaG, (GLubyte)m_deltaB);

	CCX_SAFE_DELETE(pNewZone);
	return pCopy;
}

void CCTintBy::startWithTarget(cocos2d::NSObject *pTarget)
{
	__super::startWithTarget(pTarget);

	ccColor3B color = dynamic_cast<CCRGBAProtocol*>(pTarget)->getColor();
	m_fromR = color.r;
	m_fromG = color.g;
	m_fromB = color.b;
}

void CCTintBy::update(cocos2d::ccTime time)
{
	dynamic_cast<CCRGBAProtocol*>(m_pTarget)->setColor(ccc3((GLubyte)(m_fromR + m_deltaR * time),
		                                                    (GLubyte)(m_fromG + m_deltaG * time),
															(GLubyte)(m_fromB + m_deltaB * time)));
}

CCIntervalAction* CCTintBy::reverse(void)
{
	return CCTintBy::actionWithDuration(m_fDuration, m_deltaR, m_deltaG, m_deltaB);
}

//
// DelayTime
//
void CCDelayTime::update(cocos2d::ccTime time)
{
	return;
}

CCIntervalAction* CCDelayTime::reverse(void)
{
	return CCDelayTime::actionWithDuration(m_fDuration);
}

//
// ReverseTime
//
CCReverseTime* CCReverseTime::actionWithAction(cocos2d::CCFiniteTimeAction *pAction)
{
	// casting to prevent warnings
	CCReverseTime *pReverseTime = new CCReverseTime();
	pReverseTime->initWithAction(pAction);
	pReverseTime->autorelease();

	return pReverseTime;
}

CCReverseTime* CCReverseTime::initWithAction(cocos2d::CCFiniteTimeAction *pAction)
{
	if (__super::initWithDuration(pAction->getDuration()))
	{
		m_pOther = pAction;
		pAction->retain();

		return this;
	}

	return NULL;
}

NSObject* CCReverseTime::copyWithZone(cocos2d::NSZone *pZone)
{
	NSZone* pNewZone = NULL;
	CCReverseTime* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = dynamic_cast<CCReverseTime*>(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCReverseTime();
		pZone = pNewZone = new NSZone(pCopy);
	}

	__super::copyWithZone(pZone);

	pCopy->initWithAction(dynamic_cast<CCFiniteTimeAction*>(m_pOther->copy()->autorelease()));

	CCX_SAFE_DELETE(pNewZone);
	return pCopy;
}

CCReverseTime::~CCReverseTime(void)
{
	m_pOther->release();
}

void CCReverseTime::startWithTarget(cocos2d::NSObject *pTarget)
{
	__super::startWithTarget(pTarget);
	m_pOther->startWithTarget(pTarget);
}

void CCReverseTime::stop(void)
{
	m_pOther->stop();
	__super::stop();
}

void CCReverseTime::update(cocos2d::ccTime time)
{
	m_pOther->update(1 - time);
}

CCIntervalAction* CCReverseTime::reverse(void)
{
	return static_cast<CCIntervalAction*>(m_pOther->copy()->autorelease());
}

//
// Animate
//
CCAnimate* CCAnimate::actionWithAnimation(cocos2d::CCAnimation *pAnimation)
{
	CCAnimate *pAnimate = new CCAnimate();
	pAnimate->initWithAnimation(pAnimation, true);
	pAnimate->autorelease();

	return pAnimate;
}

CCAnimate* CCAnimate::initWithAnimation(cocos2d::CCAnimation *pAnimation)
{
	assert(pAnimation != NULL);

	return initWithAnimation(pAnimation, true);
}

CCAnimate* CCAnimate::actionWithAnimation(cocos2d::CCAnimation *pAnimation, bool bRestoreOriginalFrame)
{
	CCAnimate *pAnimate = new CCAnimate();
	pAnimate->initWithAnimation(pAnimation, bRestoreOriginalFrame);
	pAnimate->autorelease();

	return pAnimate;
}

CCAnimate* CCAnimate::initWithAnimation(cocos2d::CCAnimation *pAnimation, bool bRestoreOriginalFrame)
{
	assert(pAnimation);

	if (__super::initWithDuration(pAnimation->getFrames()->count() * pAnimation->getDelay()))
	{
		m_bRestoreOriginalFrame = bRestoreOriginalFrame;
        m_pAnimation = pAnimation;
		m_pOrigFrame = NULL;

		return this;
	}

	return NULL;
}

CCAnimate* CCAnimate::actionWithDuration(cocos2d::ccTime duration, cocos2d::CCAnimation *pAnimation, bool bRestoreOriginalFrame)
{
	CCAnimate *pAnimate = new CCAnimate();
	pAnimate->initWithDuration(duration, pAnimation, bRestoreOriginalFrame);
	pAnimate->autorelease();

	return pAnimate;
}

CCAnimate* CCAnimate::initWithDuration(cocos2d::ccTime duration, cocos2d::CCAnimation *pAnimation, bool bRestoreOriginalFrame)
{
	assert(pAnimation != NULL);

	if (__super::initWithDuration(duration))
	{
		m_bRestoreOriginalFrame = bRestoreOriginalFrame;
		m_pAnimation = pAnimation;
		m_pOrigFrame = NULL;

		return this;
	}

	return NULL;
}

NSObject* CCAnimate::copyWithZone(cocos2d::NSZone *pZone)
{
	NSZone* pNewZone = NULL;
	CCAnimate* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject) 
	{
		//in case of being called at sub class
		pCopy = dynamic_cast<CCAnimate*>(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCAnimate();
		pZone = pNewZone = new NSZone(pCopy);
	}

	__super::copyWithZone(pZone);

	pCopy->initWithDuration(m_fDuration, m_pAnimation, m_bRestoreOriginalFrame);

	CCX_SAFE_DELETE(pNewZone);
	return pCopy;
}

CCAnimate::~CCAnimate(void)
{
	m_pAnimation->release();
	m_pOrigFrame->release();
}

void CCAnimate::startWithTarget(cocos2d::NSObject *pTarget)
{
	__super::startWithTarget(pTarget);
	CCSprite *pSprite = dynamic_cast<CCSprite*>(pTarget);

	m_pOrigFrame->release();

	if (m_bRestoreOriginalFrame)
	{
		m_pOrigFrame = pSprite->displayedFrame();
		m_pOrigFrame->retain();
	}
}

void CCAnimate::stop(void)
{
	if (m_bRestoreOriginalFrame)
	{
		dynamic_cast<CCSprite*>(m_pTarget)->setDisplayFrame(m_pOrigFrame);
	}

	__super::stop();
}

void CCAnimate::update(cocos2d::ccTime time)
{
	NSMutableArray<CCSpriteFrame*> *pFrames = m_pAnimation->getFrames();
	unsigned int numberOfFrames = pFrames->count();

	unsigned int idx = (unsigned int)time * numberOfFrames;

	if (idx >= numberOfFrames)
	{
		idx = numberOfFrames - 1;
	}

	CCSprite *pSprite = dynamic_cast<CCSprite*>(m_pTarget);
	if (! pSprite->isFrameDisplayed(pFrames->getObjectAtIndex(idx)))
	{
		pSprite->setDisplayFrame(pFrames->getObjectAtIndex(idx));
	}
}

CCIntervalAction* CCAnimate::reverse(void)
{
	NSMutableArray<CCSpriteFrame*> *pOldArray = m_pAnimation->getFrames();
	NSMutableArray<CCSpriteFrame*> *pNewArray = new NSMutableArray<CCSpriteFrame*>(pOldArray->count());
   
	if (pOldArray->count() > 0)
	{
		CCSpriteFrame *pElement;
		NSMutableArray<CCSpriteFrame*>::NSMutableArrayRevIterator iter;
		for (iter = pOldArray->rbegin(); iter != pOldArray->rend(); iter++)
		{
			pElement = *iter;
			if (! pElement)
			{
				break;
			}

			pNewArray->addObject(dynamic_cast<CCSpriteFrame*>(pElement->copy()->autorelease()));
		}
	}

	CCAnimation *pNewAnim = CCAnimation::animationWithName(m_pAnimation->getName(),
		m_pAnimation->getDelay(), pNewArray);

	pNewArray->release();

	return CCAnimate::actionWithDuration(m_fDuration, pNewAnim, m_bRestoreOriginalFrame);
}

}