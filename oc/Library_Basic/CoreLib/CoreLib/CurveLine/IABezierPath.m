

#import "IABezierPath.h"
#import <objc/runtime.h>

@implementation NSValue (NSValueIAPointsBezier)


+ (NSValue *)valueWithCGPoint:(CGPoint)point
{
    return [NSValue value:&point withObjCType:@encode(struct CGPoint)];
}
- (CGPoint)CGPointValue
{
    CGPoint point;
    [self getValue:&point];
    return point;
}
@end

@implementation NSBezierPath (IABezierPath)


- (void)addQuadCurveToPoint:(NSPoint)endPoint controlPoint:(NSPoint)controlPoint
{
    
    NSPoint QP0 = [self currentPoint];
    NSPoint CP3 = endPoint;
    
    NSPoint CP1 = NSMakePoint(
                              QP0.x + ((2.0 / 3.0) * (controlPoint.x - QP0.x)),
                              QP0.y + ((2.0 / 3.0) * (controlPoint.y - QP0.y))
                              );
    
    NSPoint CP2 = NSMakePoint(
                              endPoint.x + (2.0 / 3.0) * (controlPoint.x - endPoint.x),
                              endPoint.y + (2.0 / 3.0) * (controlPoint.y - endPoint.y)
                              );
    
    [self curveToPoint:CP3 controlPoint1:CP1 controlPoint2:CP2];
}
- (void)addCurveToPoint:(NSPoint)endPoint controlPoint1:(NSPoint)controlPoint1 controlPoint2:(NSPoint)controlPoint2
{
    [self curveToPoint:endPoint controlPoint1:controlPoint1 controlPoint2:controlPoint2];
}
- (void)setContractionFactor:(CGFloat)contractionFactor
{
    objc_setAssociatedObject(self, @selector(contractionFactor), @(contractionFactor), OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (CGFloat)contractionFactor
{
    id contractionFactorAssociatedObject = objc_getAssociatedObject(self, @selector(contractionFactor));
    if (contractionFactorAssociatedObject == nil) {
        return 0.7;
    }
    return [contractionFactorAssociatedObject floatValue];
}
- (void)addBezierThroughPoints:(NSArray *)pointArray
{
    NSAssert(pointArray.count > 0, @"You must give at least 1 point for drawing the curve.");
 
    if (pointArray.count < 3) {
        switch (pointArray.count) {
            case 1:
            {
                NSValue * point0Value = (NSValue*)[pointArray firstObject];
                NSPoint point0 = [point0Value pointValue];
                [self lineToPoint:point0];
            }
                break;
            case 2:
            {
                NSValue * point0Value = (NSValue*)[pointArray firstObject];
                NSPoint point0 = [point0Value pointValue];
                NSValue * point1Value = (NSValue*)[pointArray objectAtIndex:1];
                NSPoint point1 = [point1Value pointValue];
                [self addQuadCurveToPoint:point1 controlPoint:ControlPointForTheBezierCanThrough3Point(self.currentPoint, point0, point1)];
            }
                break;
            default:
                break;
        }
    }
    
    NSPoint previousPoint;// = NSMakePoint(0.0, 0.0);
    
    NSPoint previousCenterPoint;// = NSMakePoint(0.0, 0.0);;
    NSPoint centerPoint;// = NSMakePoint(0.0, 0.0);;
    CGFloat centerPointDistance = 0;
    
    CGFloat obliqueAngle = 0;
    
    NSPoint previousControlPoint1 = NSMakePoint(0.0, 0.0);;
    NSPoint previousControlPoint2 = NSMakePoint(0.0, 0.0);;
    NSPoint controlPoint1 = NSMakePoint(0.0, 0.0);;

    previousPoint = self.currentPoint;
    
    for (int i = 0; i < pointArray.count; i++) {
        
        NSValue * pointIValue = (NSValue*)[pointArray objectAtIndex:i];
        NSPoint pointI = [pointIValue pointValue];
        if (i > 0) {
            
            previousCenterPoint = CenterPointOf(self.currentPoint, previousPoint);
            centerPoint = CenterPointOf(previousPoint, pointI);
            
            centerPointDistance = DistanceBetweenPoint(previousCenterPoint, centerPoint);
            
            obliqueAngle = ObliqueAngleOfStraightThrough(centerPoint, previousCenterPoint);
            
            previousControlPoint2 = NSMakePoint(previousPoint.x - 0.5 * self.contractionFactor * centerPointDistance * cos(obliqueAngle), previousPoint.y - 0.5 * self.contractionFactor * centerPointDistance * sin(obliqueAngle));
            controlPoint1 = NSMakePoint(previousPoint.x + 0.5 * self.contractionFactor * centerPointDistance * cos(obliqueAngle), previousPoint.y + 0.5 * self.contractionFactor * centerPointDistance * sin(obliqueAngle));
        }
        
        if (i == 1) {
            
            [self addQuadCurveToPoint:previousPoint controlPoint:previousControlPoint2];
        }
        else if (i > 1 && i < pointArray.count - 1) {
        
            [self addCurveToPoint:previousPoint controlPoint1:previousControlPoint1 controlPoint2:previousControlPoint2];
        }
        else if (i == pointArray.count - 1) {
        
            [self addCurveToPoint:previousPoint controlPoint1:previousControlPoint1 controlPoint2:previousControlPoint2];
            [self addQuadCurveToPoint:pointI controlPoint:controlPoint1];
        }
        else {
        
        }
        
        previousControlPoint1 = controlPoint1;
        previousPoint = pointI;
    }
}

CGFloat ObliqueAngleOfStraightThrough(NSPoint point1, NSPoint point2)   //  [-π/2, 3π/2)
{
    CGFloat obliqueRatio = 0;
    CGFloat obliqueAngle = 0;
    
    if (point1.x > point2.x) {
    
        obliqueRatio = (point2.y - point1.y) / (point2.x - point1.x);
        obliqueAngle = atan(obliqueRatio);
    }
    else if (point1.x < point2.x) {
    
        obliqueRatio = (point2.y - point1.y) / (point2.x - point1.x);
        obliqueAngle = M_PI + atan(obliqueRatio);
    }
    else if (point2.y - point1.y >= 0) {
    
        obliqueAngle = M_PI/2;
    }
    else {
        obliqueAngle = -M_PI/2;
    }
    
    return obliqueAngle;
}

NSPoint ControlPointForTheBezierCanThrough3Point(NSPoint point1, NSPoint point2, NSPoint point3)
{
    return NSMakePoint(2 * point2.x - (point1.x + point3.x) / 2, 2 * point2.y - (point1.y + point3.y) / 2);
}

CGFloat DistanceBetweenPoint(NSPoint point1, NSPoint point2)
{
    return sqrt((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y));
}

NSPoint CenterPointOf(NSPoint point1, NSPoint point2)
{
    return NSMakePoint((point1.x + point2.x) / 2, (point1.y + point2.y) / 2);
}

@end
