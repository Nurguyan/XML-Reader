#include "xmlfileworker.h"
#include <QXmlStreamReader>
#include <QDebug>

XMLFileWorker::XMLFileWorker() : m_cancel(false){}

void XMLFileWorker::importFromFile(const QStringList &files)
{
    QList<TextEditor> result;
    for (auto i = 0; i < files.count(); ++i)
    {
        if (isCanceled()) break;

        auto& filename = files.at(i);
        emit fileChanged(filename);
        //QThread::sleep(1); //uncomment this to test progress dialog, can be removed
        TextEditor tmp;
        QFile file(filename);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            emit fileImportCompleted(filename, false);
            qDebug("Couldn't open file.");
            return;
        }

        QXmlStreamReader xml(&file);

        //Parse the XML until we reach end of it
        while(!xml.atEnd() && !xml.hasError())
        {
            // Read next element
            QXmlStreamReader::TokenType token = xml.readNext();

            //If token is just StartDocument - go to next
            if(token == QXmlStreamReader::StartDocument)
            {
                continue;
            }
            //If token is StartElement - read it
            if(token == QXmlStreamReader::StartElement)
            {
                if(xml.name() == "root")
                    continue;

                if(xml.name() == "texteditor"){
                    tmp.setName(xml.readElementText());
                }

                if(xml.name() == "fileformats"){
                    tmp.setFormats(xml.readElementText());
                }
                if(xml.name() == "encoding"){
                    tmp.setEncoding(xml.readElementText());
                }
                if(xml.name() == "hasintellisense")
                {
                    if (xml.readElementText().compare("true", Qt::CaseSensitivity::CaseInsensitive) == 0)
                    {
                        tmp.setIntellisense(true);
                    }
                    else
                    {
                        tmp.setIntellisense(false);
                    }
                }
                if(xml.name() == "hasplugins")
                {
                    if (xml.readElementText().compare("true", Qt::CaseSensitivity::CaseInsensitive) == 0)
                    {
                        tmp.setPlugins(true);
                    }
                    else
                    {
                        tmp.setPlugins(false);
                    }
                }
                if(xml.name() == "cancompile")
                {
                    if (xml.readElementText().compare("true", Qt::CaseSensitivity::CaseInsensitive) == 0)
                    {
                        tmp.setCompiles(true);
                    }
                    else
                    {
                        tmp.setCompiles(false);
                    }
                }
            }    
        }

        if(xml.hasError()){
            //qDebug() << "Cannot import: " << filename;
            emit fileImportCompleted(filename, false);
        }
        else {
            result.append(tmp);
            //qDebug() << "Import successful: " << filename;
            emit fileImportCompleted(filename, true);
        }
        //close reader and flush file
        xml.clear();
        file.close();

    }
    emit sendImportedData(result);
    emit done();
}

void XMLFileWorker::onCancel()
{
    const QMutexLocker locker(&m_mutex);
    m_cancel = true;
}

bool XMLFileWorker::isCanceled()
{
    const QMutexLocker locker(&m_mutex);
    return m_cancel;
}

void XMLFileWorker::exportToFile(const QString &filename, const TextEditor &text_editor)
{
    QFile out(filename);
    if (!out.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qCritical("Couldn't open file.");
        return;
    }
    QXmlStreamWriter stream(&out);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("root");
    stream.writeTextElement("texteditor", text_editor.getName());
    stream.writeTextElement("fileformats", text_editor.getFormats());
    stream.writeTextElement("encoding", text_editor.getEncoding());
    stream.writeTextElement("hasintellisense", text_editor.hasIntellisense() ? "true" : "false");
    stream.writeTextElement("hasplugins", text_editor.hasIntellisense() ? "true" : "false");
    stream.writeTextElement("cancompile", text_editor.hasIntellisense() ? "true" : "false");
    stream.writeEndElement();
    stream.writeEndDocument();
    emit done();
}
