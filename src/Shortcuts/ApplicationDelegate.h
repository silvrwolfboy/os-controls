#import "MenubarController.h"

@interface ApplicationDelegate : NSObject <NSApplicationDelegate> {
@private
  MenubarController* _menubarController;
  BOOL _isInitialized;
  BOOL _wasHidden;
}

@property (nonatomic, readonly) MenubarController* menubarController;

- (void)validateAccessibility;
- (void)addStatusItem;

@end
