#pragma once

#include <QObject>
#include <QPointer>

#include "Project.hpp"

class ProjectManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(
      Project *activeProject READ activeProject WRITE setActiveProject NOTIFY activeProjectChanged);

public:
    explicit ProjectManager(QObject *parent = nullptr) : QObject(parent), m_activeProject(nullptr)
    {
    }

    Project *activeProject() const { return m_activeProject; }
    void     setActiveProject(Project *project)
    {
        if (m_activeProject == project) return;

        if (!m_activeProject.isNull())
        {
            disconnect(
              m_activeProject,
              &Project::pixelAspectChanged,
              this,
              &ProjectManager::pixelAspectChanged);
        }

        m_activeProject = project;

        if (!m_activeProject.isNull())
        {
            connect(
              m_activeProject,
              &Project::pixelAspectChanged,
              this,
              &ProjectManager::pixelAspectChanged);
        }

        emit activeProjectChanged(m_activeProject);
    }

signals:
    void activeProjectChanged(Project *project);

    // Active Project signals
    void pixelAspectChanged(const qreal pixelAspectRatio);

private:
    QPointer<Project> m_activeProject;
};