#include "quiz.h"
#include <iostream>
#include <qobject.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QFile>
Quiz::Quiz() {}

void Quiz::addCategory(QString categoryName)
{
    categories.push_back(new Category(categoryName));
}

void Quiz::removeCategory(int index)
{
    categories.removeAt(index);
}

QList<Category *> Quiz::getCategories() const
{
    return categories;
}

void Quiz::addPlayer(QString playerName)
{
    players.push_back(new Player(playerName));
}

void Quiz::removePlayer(int index)
{
    players.removeAt(index);
}

void Quiz::saveQuiz(QString quizname)
{
    if(quizname == ""){
        quizname = "NeuesQuiz";
    }

    QJsonObject quizJson;

    QJsonArray playersJson;
    QJsonArray categoriesJson;

    for(int i = 0; i < players.size();i++){
        playersJson.append(players[i]->getName());
    }
    for(int i = 0; i < categories.size(); i++){
        QJsonObject categoryJson;
        QJsonArray categoryQuestions;

        categoryJson["name"] = categories[i]->getName();

        for(int j = 0; j < categories[i]->getQuestions().size(); j++){
            Question* currQuestion = categories[i]->getQuestions()[j];

            QJsonObject questionJson;
            questionJson["question"] = currQuestion->getQuestionText();
            questionJson["points"] = currQuestion->getPoints();
            questionJson["answer"] = currQuestion->getAnswer();
            questionJson["media"] = currQuestion->getContentSource();

            categoryQuestions.append(questionJson);
        }

        categoryJson["questions"] = categoryQuestions;
        categoriesJson.append(categoryJson);
    }

    quizJson["players"] = playersJson;
    quizJson["categories"] = categoriesJson;

    QJsonDocument jsonDoc(quizJson);

    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QFile file(documentsPath + "/" + quizname + ".json");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("Couldn't open file for writing.");
        return;
    }

    file.write(jsonDoc.toJson(QJsonDocument::Indented)); // or Compact
    file.close();
}

void Quiz::loadQuiz(QString filepath)
{
    players.clear();
    categories.clear();

    QFile datei(filepath);
    if (!datei.open(QIODevice::ReadOnly)){
        std::cerr << "Datei konnte nicht geoeffnet werden";
        return;
    }
    QByteArray content = datei.readAll();
    datei.close();

    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(content,&parseError);
    QJsonObject json = document.object();

    QJsonArray playersJson = json["players"].toArray();
    QJsonArray categoriesJson = json["categories"].toArray();

    for(int i = 0; i < playersJson.size();i++){
        players.push_back(new Player(playersJson[i].toString()));
    }
    for(int i = 0; i < categoriesJson.size(); i++){
        QJsonObject categoryJson = categoriesJson[i].toObject();
        QJsonArray questionsJson = categoryJson["questions"].toArray();

        Category* newCategory = new Category(categoryJson["name"].toString());

        for(int j = 0;j < questionsJson.size();j++){
            QJsonObject questionJson = questionsJson[j].toObject();
            newCategory->addQuestion(questionJson["question"].toString(), questionJson["points"].toInt(),questionJson["answer"].toString());
            int questionIndex = newCategory->getQuestions().size()-1;
            newCategory->getQuestions().at(questionIndex)->setContentSource(questionJson["media"].toString());
        }
        categories.push_back(newCategory);
    }
}

QList<Player *> Quiz::getPlayers() const
{
    return players;
}

