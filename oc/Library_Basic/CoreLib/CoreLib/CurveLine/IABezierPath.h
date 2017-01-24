
#import <Quartz/Quartz.h>
#import <Cocoa/Cocoa.h>
@interface NSValue (NSValueIAPointsBezier)

+ (NSValue *)valueWithCGPoint:(CGPoint)point;

- (CGPoint)CGPointValue;

@end

@interface NSBezierPath(IABezierPath)
{

}
CGFloat ObliqueAngleOfStraightThrough(NSPoint point1, NSPoint point2);
NSPoint ControlPointForTheBezierCanThrough3Point(NSPoint point1, NSPoint point2, NSPoint point3);
CGFloat DistanceBetweenPoint(NSPoint point1, NSPoint point2);
NSPoint CenterPointOf(NSPoint point1, NSPoint point2);
/**
 *  The curve‘s bend level. The good value is about 0.6 ~ 0.8. The default and recommended value is 0.7.
 */
@property (nonatomic) CGFloat contractionFactor;



/**
 *  You must wrap CGPoint struct to NSValue object.
 *
 *  @param pointArray Points you want to through. You must give at least 1 point for drawing curve.
 */

- (void)addBezierThroughPoints:(NSArray *)pointArray ;
- (void)addQuadCurveToPoint:(NSPoint)endPoint controlPoint:(NSPoint)controlPoint;
- (void)addCurveToPoint:(NSPoint)endPoint controlPoint1:(NSPoint)controlPoint1 controlPoint2:(NSPoint)controlPoint2;

@end

