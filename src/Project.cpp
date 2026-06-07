#include "Project.hpp"

#include <QFile>
#include <QException>
#include "ColorSpace.hpp"

Project::Project(const QString &video_path) : m_video_path(video_path)
{
}

Project::~Project()
{
}

Project Project::from_json(const QString &filepath)
{
}

void Project::save()
{
}

void Project::stepFrame(int frames)
{
}
void Project::setTimestamp(int64_t timestamp)
{
}

bool Project::has_changed()
{ return this->m_changed; }

QColorSpace Project::colorspace()
{ return generateColorSpace(this->m_video_primaries, this->m_video_transfer); }
