//
//  TCocoaWorkshopController.mm
//  Einstein
//
//  Created by Matthias Melcher on 2/27/2017.
//

#import "TCocoaWorkshopController.h"
//#import "Monitor/TMacMonitor.h"
#import "TCocoaUserDefaults.h"
#import "TMacWorkshop.h"
//#import "TMonitorCore.h"

@interface TCocoaWorkshopController ()
//- (void)update;
@end

@implementation TCocoaWorkshopController

//- (void)addHistoryLine:(NSString *)line type:(int)type
//{
//	[view addHistoryLine:line type:type];
//}


- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	[[self window] setDelegate:nil];	// so windowWillClose doesn't get called
	[[NSNotificationCenter defaultCenter] removeObserver:self];
}


- (void)awakeFromNib
{
	[[self window] setDelegate:self];

//	[view setController:self];
//
//	[self update];

	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillTerminate:) name:NSApplicationWillTerminateNotification object:nil];
}


//- (void)executeCommand:(NSString*)command
//{
//	[self addHistoryLine:[NSString stringWithFormat:@"> %@", command] type:MONITOR_LOG_USER_INPUT];
//
//	monitor->ExecuteCommand([command UTF8String]);
//	[self performSelector:@selector(update) withObject:nil afterDelay:0.0];
//
//	if ( [command isEqualToString:@"run"] )
//	{
//		[stopStartButton setTitle:@"Stop"];
//	}
//	else if ( [command isEqualToString:@"stop"] )
//	{
//		[stopStartButton setTitle:@"Run"];
//	}
//}


- (void)setWorkshop:(TMacWorkshop*)inWorkshop
{
	workshop = inWorkshop;
	workshop->SetController(self);
}


- (void)showWindow:(id)sender
{
	[super showWindow:sender];

	[[NSUserDefaults standardUserDefaults] setBool:YES forKey:kOpenWorkshopAtLaunch];
	[[NSUserDefaults standardUserDefaults] synchronize];
}


- (void)compileAndRun:(id)sender
{
	workshop->CompileAndRun();
}


//- (IBAction)stepInto:(id)sender
//{
//	monitor->ExecuteCommand("step");
//	[self performSelector:@selector(update) withObject:nil afterDelay:0.0];
//}


//- (IBAction)stepOver:(id)sender
//{
//	monitor->ExecuteCommand("trace");
//	[self performSelector:@selector(update) withObject:nil afterDelay:0.0];
//}

//- (void)stopStart:(id)sender
//{
//	if ( monitor )
//	{
//		if ( monitor->IsHalted() )
//		{
//			monitor->ExecuteCommand("run");
//		}
//		else
//		{
//			monitor->ExecuteCommand("stop");
//		}
//
//		[self performSelector:@selector(update) withObject:nil afterDelay:0.0];
//	}
//	else
//	{
//		[self performSelector:@selector(update) withObject:nil afterDelay:0.0];
//	}
//}


//- (void)update
//{
//	if ( monitor )
//	{
//		[view updateWithMonitor:monitor];
//
//		if ( monitor->IsHalted() )
//		{
//			[stepIntoButton setEnabled:YES];
//			[stepOverButton setEnabled:YES];
//			[stopStartButton setTitle:@"Run"];
//		}
//		else
//		{
//			[stepIntoButton setEnabled:NO];
//			[stepOverButton setEnabled:NO];
//			[stopStartButton setTitle:@"Stop"];
//		}
//
//		[stopStartButton setEnabled:YES];
//	}
//	else
//	{
//		[stepIntoButton setEnabled:NO];
//		[stepOverButton setEnabled:NO];
//		[stopStartButton setEnabled:NO];
//		[stopStartButton setTitle:@"Run"];
//	}
//}


- (void)windowWillClose:(NSNotification *)notification
{
	[[NSUserDefaults standardUserDefaults] setBool:NO forKey:kOpenWorkshopAtLaunch];
	[[NSUserDefaults standardUserDefaults] synchronize];
}


@end
