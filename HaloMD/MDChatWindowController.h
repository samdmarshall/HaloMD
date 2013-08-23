/*
 * Copyright (c) 2013, Null <foo.null@yahoo.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//
//  MDChatWindowController.h
//  HaloMD
//
//  Created by null on 2/23/13.
//

#import <Cocoa/Cocoa.h>
#import <Ruby/ruby.h>

#define CHAT_PLAY_MESSAGE_SOUNDS @"CHAT_PLAY_MESSAGE_SOUNDS"
#define CHAT_SHOW_MESSAGE_RECEIVE_NOTIFICATION @"CHAT_SHOW_MESSAGE_RECEIVE_NOTIFICATION"

@class WebView;

@interface MDChatWindowController : NSWindowController
{
	IBOutlet WebView *webView;
	IBOutlet NSTextView *textView;
	IBOutlet NSSplitView *chatSplitView;
	IBOutlet NSSplitView *rosterSplitView;
	IBOutlet NSTableView *rosterTableView;
	VALUE chatting;
	VALUE chattingClass;
	NSTimer *chatTimer;
	int previousMaxScroll;
	uint64_t numberOfUnreadMentions;
	NSMutableArray *roster;
	NSString *myNick;
	BOOL willTerminate;
}

@property (nonatomic, copy) NSString *myNick;

- (void)cleanup;

@end
