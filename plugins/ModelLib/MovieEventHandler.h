#ifndef MOVIEEVENTHANDLER_H
#define MOVIEEVENTHANDLER_H

#include "osgGA/GUIEventHandler"
#include "osg/ImageStream"
#include "osg/Geode"
#include "osg/Texture2D"
#include "osg/TextureRectangle"

class MovieEventHandler : public osgGA::GUIEventHandler
{
public:

	MovieEventHandler():_trackMouse(false) {}

	void setMouseTracking(bool track) { _trackMouse = track; }
	bool getMouseTracking() const { return _trackMouse; }

	void set(osg::Node* node);
	void addNode(osg::Node* node);
	void addImage(osg::Image* image);

	void Restart();
	void play();
	void pause();
	void ToggleLooping();
	void IncreaseSpeed();
	void DecreaseSpeed();

	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor* nv);

	virtual void getUsage(osg::ApplicationUsage& usage) const;

	typedef std::vector< osg::observer_ptr<osg::ImageStream> > ImageStreamList;

protected:

	virtual ~MovieEventHandler() {}

	class FindImageStreamsVisitor : public osg::NodeVisitor
	{
	public:
		FindImageStreamsVisitor(ImageStreamList& imageStreamList):osg::NodeVisitor(TRAVERSE_ALL_CHILDREN),
			_imageStreamList(imageStreamList) {}

		virtual void apply(osg::Geode& geode)
		{
			apply(geode.getStateSet());

			for(unsigned int i=0;i<geode.getNumDrawables();++i)
			{
				apply(geode.getDrawable(i)->getStateSet());
			}

			traverse(geode);
		}

		virtual void apply(osg::Node& node)
		{
			apply(node.getStateSet());
			traverse(node);
		}

		inline void apply(osg::StateSet* stateset)
		{
			if (!stateset) return;

			osg::StateAttribute* attr = stateset->getTextureAttribute(0,osg::StateAttribute::TEXTURE);
			if (attr)
			{
				osg::Texture2D* texture2D = dynamic_cast<osg::Texture2D*>(attr);
				if (texture2D) apply(dynamic_cast<osg::ImageStream*>(texture2D->getImage()));

				osg::TextureRectangle* textureRec = dynamic_cast<osg::TextureRectangle*>(attr);
				if (textureRec) apply(dynamic_cast<osg::ImageStream*>(textureRec->getImage()));
			}
		}

		inline void apply(osg::ImageStream* imagestream)
		{
			if (imagestream)
			{
				for(ImageStreamList::iterator itr=_imageStreamList.begin(); itr!=_imageStreamList.end(); )
				{
					if( !(*itr).valid() )
					{
						itr = _imageStreamList.erase(itr);
						continue;
					}

					if( *itr==imagestream )
						return;

					itr++;
				}

				_imageStreamList.push_back(imagestream);
			}
		}

		ImageStreamList& _imageStreamList;

	protected:

		FindImageStreamsVisitor& operator = (const FindImageStreamsVisitor&) { return *this; }

	};


	bool            _trackMouse;
	ImageStreamList _imageStreamList;
	unsigned int    _seekIncr;

};

#endif // MOVIEEVENTHANDLER_H
