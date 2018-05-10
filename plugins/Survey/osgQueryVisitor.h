#pragma once
#include "osg/NodeVisitor"
#include <vector>
#include "osg/Vec3d"
#include "osg/ComputeBoundsVisitor"
class COsgQueryVisitor : public osg::NodeVisitor
{
public:
	std::vector<std::string> vecName;
	std::vector<osg::BoundingBox> vecBox;
	COsgQueryVisitor() : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) 
	{
		vecName.clear();
		vecBox.clear();
	}

	//判断点在box内
	bool inBox(osg::Vec3d pt,osg::BoundingBox& box,double& disToCenter)
	{
		osg::Vec3d p;
		osg::Vec3d center = box.center();
		double xmax = box.xMax();
		double xmin = box.xMin();
		double ymax = box.yMax();
		double ymin = box.yMin();
		bool b = isContain(osg::Vec3d(xmin,ymin,0),osg::Vec3d(xmin,ymax,0),osg::Vec3d(xmax,ymax,0),osg::Vec3d(xmax,ymin,0),pt);
		if (b && pt.z() >= box.zMin() && pt.z() <= box.zMax())
		{
			disToCenter = (pt - center).length();
			return true;
		}
		return false;
	}
	bool inPolygon2D(osg::Vec3d pt, std::vector<osg::Vec3d>& vecPoly)
	{
		bool isInside = false; 
		int count = 0;   

		double px = pt.x();
		double py = pt.y();
		double linePoint1x = pt.x();   
		double linePoint1y = pt.y();   
		double linePoint2x = -99999999.9;//0;       
		double linePoint2y = pt.y();      

		for(int i = 0; i < vecPoly.size()-1; i++)
		{
			double cx1 = vecPoly[i].x();   
			double cy1 = vecPoly[i].y();    
			double cx2 = vecPoly[i+1].x();    
			double cy2 = vecPoly[i+1].y();  

			if(IsPointOnLine(px, py, cx1, cy1, cx2, cy2))    
			{
				return true; 		
			}

			if (fabs(cy2 - cy1) < 0.000001)   //平行则不相交
			{
				continue;   
			}   

			if (IsPointOnLine(cx1, cy1, linePoint1x, linePoint1y, linePoint2x, linePoint2y)) 
			{   
				if (cy1 > cy2)   
				{
					count++; 
				}
			} 
			else if (IsPointOnLine(cx2, cy2, linePoint1x, linePoint1y, linePoint2x, linePoint2y))
			{ 
				if (cy2 > cy1)  
				{
					count++; 
				}
			} 
			else if (IsIntersect(cx1, cy1, cx2, cy2, linePoint1x, linePoint1y, linePoint2x, linePoint2y))   //已经排除平行的情况
			{ 
				count++;  
			}   
		}

		if (count % 2 == 1) 
		{   
			isInside = true;
		}   

		return isInside;		
	}
protected:
	virtual void apply(osg::MatrixTransform& trans)
	{
		if (trans.getName()!=std::string("场景根"))
		{
			vecName.push_back(trans.getName());
			osg::ComputeBoundsVisitor boundVisitor;
			trans.accept(boundVisitor);
			vecBox.push_back(boundVisitor.getBoundingBox());
		}
		traverse(trans);
	}

	double Multiply(osg::Vec3d p1, osg::Vec3d p2, osg::Vec3d p0)
	{
		return ((p1.x() - p0.x()) * (p2.y() - p0.y()) - (p2.x() - p0.x()) * (p1.y() - p0.y()));
	}

	bool isContain(osg::Vec3d mp1,osg::Vec3d mp2,osg::Vec3d mp3,osg::Vec3d mp4,osg::Vec3d mp)
	{
		if (Multiply(mp, mp1, mp2) * Multiply(mp,mp4, mp3) <= 0&& Multiply(mp, mp4, mp1) * Multiply(mp, mp3, mp2) <= 0)
			return true;

		return false;
	}
	// 计算叉乘 |P0P1| × |P0P2| 

	//判断点在线段上
	bool IsPointOnLine(double px0, double py0, double px1, double py1, double px2, double py2)
	{
		bool flag = false;   
		double d1 = (px1 - px0) * (py2 - py0) - (px2 - px0) * (py1 - py0);
		if ((abs(d1) < 0.000001)&&((px0 - px1) * (px0 - px2) <= 0)&&((py0 - py1) * (py0 - py2) <= 0)) 
		{   
			flag = true;   
		}
		return flag;   
	}

	//判断两线段相交
	bool IsIntersect(double px1, double py1, double px2, double py2, double px3, double py3, double px4, double py4) 
	{         
		bool flag = false;   
		double d = (px2 - px1) * (py4 - py3) - (py2 - py1) * (px4 - px3);   
		if (d != 0) 
		{   
			double r = ((py1 - py3) * (px4 - px3) - (px1 - px3) * (py4 - py3))/d;   
			double s = ((py1 - py3) * (px2 - px1) - (px1 - px3) * (py2 - py1))/d;   
			if ((r >= 0) && (r <= 1) && (s >= 0) && (s <= 1)) 
			{ 
				flag = true;   
			}	
		} 		
		return flag; 
	}   
};


