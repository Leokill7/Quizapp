#include "quizzui.h"
#include "./ui_quizzui.h"
#include <QPushButton>
#include <QFileDialog>
#include <QStyle>
#include <qstandardpaths.h>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSlider>
#include <QVideoWidget>
QuizzUI::QuizzUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QuizzUI)
{
    ui->setupUi(this);

    quiz = new Quiz();
    /*quiz->addPlayer("Vito");
    quiz->addPlayer("Leonard");
    quiz->addPlayer("Julian");

    quiz->addCategory("Geschichte");
    quiz->addCategory("Geographie");
    quiz->addCategory("Mathematik");

    quiz->getCategories()[0]->addQuestion("Wann war die französische Revolution?",100,"1779");
    quiz->getCategories()[0]->getQuestions()[0]->setContentSource("C:\\Users\\Leonard\\Downloads\\deep-abstract-ambient_snowcap-401656.mp3");
    quiz->getCategories()[0]->addQuestion("Wann war die französische Revolution?",100,"1779");
    quiz->getCategories()[0]->getQuestions()[1]->setContentSource("C:\\Users\\Leonard\\Downloads\\xy.mp4");
    quiz->getCategories()[0]->addQuestion("Wann war die französische Revolution?",100,"1779");

    quiz->getCategories()[1]->addQuestion("Was ist 1+1",100,"2");
    quiz->getCategories()[1]->addQuestion("Was ist 3*3*3?",200,"27");
    quiz->getCategories()[1]->addQuestion("Was ist 4 zum Quadrat",300,"16");

    quiz->getCategories()[2]->addQuestion("Wann war die französische Revolution?",100,"1779");
    quiz->getCategories()[2]->addQuestion("Wann war die französische Revolution?",100,"1779");
    quiz->getCategories()[2]->addQuestion("Wann war die französische Revolution?",100,"1779");*/

    //Nutzen zum fertig builden
    //"C:\Qt\6.9.1\mingw_64\bin\windeployqt.exe" Quizapp.exe

    setWindowTitle("Quizapp");

    connect(ui->actionStarten, &QAction::triggered, this, [=](){startQuiz();});
    connect(ui->actionStopeen, &QAction::triggered, this, [=](){
        stopQuiz();
    });
    connect(ui->actionSpeichern, &QAction::triggered, this, [=](){
        QWidget* quizNameInputWidget = new QWidget();
        quizNameInputWidget->setWindowModality(Qt::ApplicationModal);

        QVBoxLayout* container = new QVBoxLayout(quizNameInputWidget);
        QHBoxLayout* inputContainer = new QHBoxLayout();
        QLabel* descrLabel = new QLabel("Quiz Name:");
        QLineEdit* nameInput = new QLineEdit();
        inputContainer->addWidget(descrLabel);
        inputContainer->addWidget(nameInput);

        QPushButton* saveButton = new QPushButton("Speichern");
        connect(saveButton, &QPushButton::clicked, this, [=](){
            quiz->saveQuiz(nameInput->text());
            quizNameInputWidget->close();
        });

        container->addLayout(inputContainer);
        container->addWidget(saveButton);
        quizNameInputWidget->show();
    });
    connect(ui->actionLaden, &QAction::triggered, this, [=](){
        stopQuiz();
        QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        QString fileName = QFileDialog::getOpenFileName(this,"Open File",documentsPath,"JSON Files (*.json);");
        quiz->loadQuiz(fileName);
        startEditing();
    });

    startEditing();
}

QuizzUI::~QuizzUI()
{
    delete ui;
}

void QuizzUI::showPlayerStats(QHBoxLayout *playerStatsHorizontalLayout)
{
    clearLayout(playerStatsHorizontalLayout);

    QVBoxLayout* descrContainer = new QVBoxLayout();
    QLabel* emptyLabel = new QLabel();
    QLabel* pointsDescrLabel = new QLabel("Punkte");
    QLabel* correctAnswersDescrLabel = new QLabel("Beantwortet");
    descrContainer->addWidget(emptyLabel);
    descrContainer->addWidget(pointsDescrLabel);
    descrContainer->addWidget(correctAnswersDescrLabel);
    playerStatsHorizontalLayout->addLayout(descrContainer);

    for(int i = 0; i < quiz->getPlayers().size();i++){
        Player* currPlayer = quiz->getPlayers().at(i);
        QVBoxLayout* playerStatsLayout = new QVBoxLayout();

        QLabel* playerName = new QLabel(currPlayer->getName());
        playerStatsLayout->addWidget(playerName, 0,Qt::AlignHCenter);

        QHBoxLayout* playerPointsContainer = new QHBoxLayout();

        QPushButton* reducePointsButton = createMiniButton("-");
        connect(reducePointsButton, &QPushButton::clicked, this, [=](){
            currPlayer->setPoints(currPlayer->getPoints()-100);
            showPlayerStats(playerStatsHorizontalLayout);
        });
        playerPointsContainer->addWidget(reducePointsButton);

        QLabel* pointsLabel = new QLabel(QString::number(currPlayer->getPoints()));
        playerPointsContainer->addWidget(pointsLabel, 0,Qt::AlignHCenter);

        QPushButton* addPointsButton = createMiniButton("+");
        connect(addPointsButton, &QPushButton::clicked, this, [=](){
            currPlayer->setPoints(currPlayer->getPoints()+100);
            showPlayerStats(playerStatsHorizontalLayout);
        });
        playerPointsContainer->addWidget(addPointsButton);

        playerStatsLayout->addLayout(playerPointsContainer);
        playerStatsHorizontalLayout->addLayout(playerStatsLayout);

        QLabel* correctAnswersLabel = new QLabel(QString::number(currPlayer->getCorrectAnswers()));
        playerStatsLayout->addWidget(correctAnswersLabel, 0,Qt::AlignHCenter);
    }

    playerStatsHorizontalLayout->addSpacerItem(new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
}
void QuizzUI::startQuiz()
{
    clearLayout(ui->categoryContainer);

    QHBoxLayout* categories = new QHBoxLayout();
    QHBoxLayout* playerStatsHorizontalLayout = new QHBoxLayout();

    for(int i = 0; i < quiz->getCategories().size();i++){
        Category* currCategory = quiz->getCategories().at(i);

        QVBoxLayout* container = new QVBoxLayout();
        QLabel* categoryName = new QLabel(currCategory->getName());
        container->addWidget(categoryName, 0,Qt::AlignHCenter);

        for(int j = 0; j < currCategory->getQuestions().size();j++){
            Question* currQuestion = currCategory->getQuestions().at(j);

            QPushButton* selectQuestionButton = new QPushButton(QString::number(currQuestion->getPoints()));
            if(currQuestion->getAnswered()){
                selectQuestionButton->setDisabled(true);
            }
            connect(selectQuestionButton, &QPushButton::clicked, this, [=](){
                Question* currQuestion = quiz->getCategories().at(i)->getQuestions()[j];
                clearLayout(ui->categoryContainer);

                QVBoxLayout* questionContainer = new QVBoxLayout();
                ui->categoryContainer->addLayout(questionContainer);

                QLabel* questionDescr = new QLabel(currCategory->getName() + " "+ QString::number(currQuestion->getPoints()));
                questionDescr->setMaximumHeight(40);
                questionContainer->addWidget(questionDescr, 0,Qt::AlignTop | Qt::AlignHCenter);

                QLabel* questionText = new QLabel(currQuestion->getQuestionText());
                questionText->setProperty("class", "question");
                questionContainer->addWidget(questionText, 0,Qt::AlignTop | Qt::AlignHCenter);

                if (isImage(currQuestion->getContentSource()))
                {
                    QLabel* imageLabel = new QLabel("");
                    QPixmap pix(currQuestion->getContentSource());
                    imageLabel->setPixmap(pix);
                    questionContainer->addWidget(imageLabel, 0,Qt::AlignHCenter);
                }else if (isAudio(currQuestion->getContentSource())||isVideo(currQuestion->getContentSource())){
                    QVBoxLayout* mediaPlayerLayout = createMediaPlayer(currQuestion->getContentSource());

                    questionContainer->addLayout(mediaPlayerLayout);
                }

                if(currQuestion->getAnswer() != ""){
                    QLabel* answerText = new QLabel(currQuestion->getAnswer());
                    answerText->setVisible(false);
                    answerText->setProperty("class", "question");
                    QPushButton* revealAnserButton = new QPushButton("Antwort");
                    connect(revealAnserButton, &QPushButton::clicked, this, [=](){
                        revealAnserButton->setVisible(false);
                        answerText->setVisible(true);
                    });

                    questionContainer->addWidget(answerText, 0,Qt::AlignHCenter);
                    questionContainer->addWidget(revealAnserButton);
                }

                QHBoxLayout* playersContainer = new QHBoxLayout();
                for(int k = 0; k < quiz->getPlayers().size();k++){
                    Player* currPlayer = quiz->getPlayers().at(k);

                    QPushButton* setWinningPlayerButton = new QPushButton(currPlayer->getName());
                    connect(setWinningPlayerButton, &QPushButton::clicked, this, [=](){
                        currPlayer->addPoints(currQuestion->getPoints());
                        currPlayer->addCorrectAnswer();
                        currQuestion->setAnswered(true);
                        startQuiz();
                    });
                    playersContainer->addWidget(setWinningPlayerButton);
                }
                QPushButton* noWinnerButton = new QPushButton("Keiner");
                connect(noWinnerButton, &QPushButton::clicked, this, [=](){
                    currQuestion->setAnswered(true);
                    startQuiz();
                });
                playersContainer->addWidget(noWinnerButton);
                questionContainer->addLayout(playersContainer);

                QPushButton* saveButton = new QPushButton("Abbrechen");
                questionContainer->addWidget(saveButton);
                connect(saveButton, &QPushButton::clicked, this, [=](){
                    startQuiz();
                });
            });

            container->addWidget(selectQuestionButton);
        }

        container->addSpacerItem(new QSpacerItem( 40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

        categories->addLayout(container);
    }
    ui->categoryContainer->addLayout(categories);

    ui->categoryContainer->addLayout(playerStatsHorizontalLayout);
    showPlayerStats(playerStatsHorizontalLayout);
}

void QuizzUI::stopQuiz()
{
    for(int i = 0; i < quiz->getPlayers().size();i++){
        quiz->getPlayers().at(i)->setPoints(0);
        quiz->getPlayers().at(i)->setCorrectAnswers(0);
    }
    for(int i = 0; i < quiz->getCategories().size();i++){
        Category* currCateory = quiz->getCategories().at(i);
        for(int j = 0; j < currCateory->getQuestions().size();j++){
            currCateory->getQuestions().at(j)->setAnswered(false);
        }
    }

    startEditing();
}

void QuizzUI::startEditing()
{
    clearLayout(ui->categoryContainer);

    QHBoxLayout* categories = new QHBoxLayout();
    for(int i = 0; i < quiz->getCategories().size();i++){
        Category* currCategory = quiz->getCategories().at(i);

        QVBoxLayout* container = new QVBoxLayout();
        QHBoxLayout* headerContainer = new QHBoxLayout();
        QLabel* categoryName = new QLabel(currCategory->getName());
        QPushButton* removeCategoryButton = createMiniButton("-");

        connect(removeCategoryButton, &QPushButton::clicked, this, [=](){
            quiz->removeCategory(i);
            startEditing();
        });

        headerContainer->addWidget(categoryName, 0,Qt::AlignHCenter);
        headerContainer->addWidget(removeCategoryButton);

        container->addLayout(headerContainer);

        for(int j = 0; j < currCategory->getQuestions().size();j++){
            Question* currQuestion = currCategory->getQuestions().at(j);

            QHBoxLayout* questionContainer = new QHBoxLayout();
            QPushButton* selectQuestionButton = new QPushButton(QString::number(currQuestion->getPoints()));
            connect(selectQuestionButton, &QPushButton::clicked, this, [=](){
                Question* questionToEdit = quiz->getCategories().at(i)->getQuestions()[j];

                questionEditingMenu(quiz->getCategories().at(i)->getQuestions()[j], i);

                startEditing();
            });
            questionContainer->addWidget(selectQuestionButton);

            QPushButton* removeQuestionButton = createMiniButton("-");
            connect(removeQuestionButton, &QPushButton::clicked, this, [=](){
                quiz->getCategories().at(i)->removeQuestion(j);
                startEditing();
            });
            questionContainer->addWidget(removeQuestionButton);
            container->addLayout(questionContainer);
        }

        QPushButton* addQuestionButton = new QPushButton("Frage hinzufügen");
        connect(addQuestionButton, &QPushButton::clicked, this, [=](){
            questionEditingMenu(nullptr, i);
            startEditing();
        });
        container->addWidget(addQuestionButton);

        container->addSpacerItem(new QSpacerItem( 40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

        categories->addLayout(container);
    }

    QVBoxLayout* addCategoryContainer = new QVBoxLayout();
    QPushButton* addCategoryButton = new QPushButton("Kategorie hinzufügen");
    connect(addCategoryButton, &QPushButton::clicked, this, [=](){
        addCategory();
    });
    addCategoryContainer->addWidget(addCategoryButton);
    addCategoryContainer->addSpacerItem(new QSpacerItem( 40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    categories->addLayout(addCategoryContainer);
    ui->categoryContainer->addLayout(categories);

    QHBoxLayout* playersContainer = new QHBoxLayout();
    QHBoxLayout* addPlayerContainer = new QHBoxLayout();
    QLineEdit* playernameInput = new QLineEdit();
    playernameInput->setMaximumWidth(120);
    addPlayerContainer->addWidget(playernameInput);
    QPushButton* addPlayerButton = new QPushButton("Spieler hinzufügen");
    connect(addPlayerButton, &QPushButton::clicked, this, [=](){
        quiz->addPlayer(playernameInput->text());
        startEditing();
    });
    addPlayerButton->setMaximumWidth(160);
    addPlayerContainer->addWidget(addPlayerButton);
    playersContainer->addLayout(addPlayerContainer);

    for(int i = 0; i < quiz->getPlayers().size();i++){
        Player* currPlayer = quiz->getPlayers().at(i);

        QLabel* playername = new QLabel(currPlayer->getName());
        playersContainer->addWidget(playername,0,Qt::AlignRight);
        QPushButton* removePlayerButton = createMiniButton("-");
        connect(removePlayerButton, &QPushButton::clicked, this, [=](){
            quiz->removePlayer(i);
            startEditing();
        });
        playersContainer->addWidget(removePlayerButton);
    }

    ui->categoryContainer->addLayout(playersContainer);
}

void QuizzUI::questionEditingMenu(Question *questionToEdit, int categoryIndex)
{
    QWidget* questionOptions = new QWidget();
    questionOptions->setWindowModality(Qt::ApplicationModal);

    QVBoxLayout* container = new QVBoxLayout(questionOptions);

    QHBoxLayout* questionInputContainer = new QHBoxLayout();
    QLabel* questionLabel = new QLabel("Frage");
    questionInputContainer->addWidget(questionLabel);
    QLineEdit* questionInput= new QLineEdit();
    questionInput->setText(questionToEdit?questionToEdit->getQuestionText():"");
    questionInputContainer->addWidget(questionInput);
    container->addLayout(questionInputContainer);

    QHBoxLayout* answerInputContainer = new QHBoxLayout();
    QLabel* answerlabel = new QLabel("Antwort");
    answerInputContainer->addWidget(answerlabel);
    QLineEdit* answerInput= new QLineEdit();
    answerInput->setText(questionToEdit?questionToEdit->getAnswer():"");
    answerInputContainer->addWidget(answerInput);
    container->addLayout(answerInputContainer);

    QHBoxLayout* pointsInputContainer = new QHBoxLayout();
    QLabel* pointsLabel = new QLabel("Punkte");
    pointsInputContainer->addWidget(pointsLabel);
    QLineEdit* pointsInput= new QLineEdit();
    pointsInput->setText(QString::number(questionToEdit?questionToEdit->getPoints():0));
    pointsInputContainer->addWidget(pointsInput);
    container->addLayout(pointsInputContainer);

    QPushButton* addMediaButton = new QPushButton("Medien hinzufügen");
    QLabel* mediaSourceLabel = new QLabel();
    connect(addMediaButton, &QPushButton::clicked, this, [=](){
        QString fileName = QFileDialog::getOpenFileName(
            this,
            "Open File",
            "",
            "All Files (*)"
            );
        mediaSourceLabel->setText(fileName);
    });
    container->addWidget(addMediaButton);
    container->addWidget(mediaSourceLabel);
    if(questionToEdit){
        mediaSourceLabel->setText(questionToEdit->getContentSource());
    }

    QPushButton* saveButton = new QPushButton("Speichern");
    container->addWidget(saveButton);
    connect(saveButton, &QPushButton::clicked, this, [=](){
        if(questionToEdit){
            questionToEdit->setQuestionText(questionInput->text());
            questionToEdit->setPoints(pointsInput->text().toInt());
            questionToEdit->setAnswer(answerInput->text());
            questionToEdit->setContentSource(mediaSourceLabel->text());
        }else{
            quiz->getCategories().at(categoryIndex)->addQuestion(questionInput->text(),pointsInput->text().toInt(),answerInput->text());
            int quizIndex = quiz->getCategories().at(categoryIndex)->getQuestions().size()-1;
            quiz->getCategories().at(categoryIndex)->getQuestions().at(quizIndex)->setContentSource(mediaSourceLabel->text());
        }

        startEditing();
        questionOptions->close();
    });

    questionOptions->show();
}

void QuizzUI::clearLayout(QLayout *layout) {
    //Pevent crsahing when deleting player while running
    if(player){
        player->stop();
    }

    QLayoutItem* child;
    while ( layout->count() != 0 ) {
        child = layout->takeAt ( 0 );
        if ( child->layout() != 0 ) {
            clearLayout ( child->layout() );
        } else if ( child->widget() != 0 ) {
            delete child->widget();
        }

        delete child;
    }
}

void QuizzUI::addCategory()
{
    QWidget* categoryOptions = new QWidget();
    categoryOptions->setWindowModality(Qt::ApplicationModal);

    QVBoxLayout* container = new QVBoxLayout(categoryOptions);

    QHBoxLayout* categoyInputContainer = new QHBoxLayout();

    QLabel* label = new QLabel("Kategorie Name",categoryOptions);
    categoyInputContainer->addWidget(label);

    QLineEdit* input = new QLineEdit(categoryOptions);
    categoyInputContainer->addWidget(input);

    container->addLayout(categoyInputContainer);

    QPushButton* saveButton = new QPushButton("Hinzufügen");
    container->addWidget(saveButton);
    connect(saveButton, &QPushButton::clicked, this, [=](){
        quiz->addCategory(input->text());
        startEditing();

        categoryOptions->close();
    });

    categoryOptions->show();
}

bool QuizzUI::isImage(QString filetype)
{
    if(filetype.endsWith(".jpeg", Qt::CaseInsensitive)||
        filetype.endsWith(".jpg", Qt::CaseInsensitive)||
        filetype.endsWith(".png", Qt::CaseInsensitive)||
        filetype.endsWith(".psd", Qt::CaseInsensitive)||
        filetype.endsWith(".pdf", Qt::CaseInsensitive)||
        filetype.endsWith(".heic", Qt::CaseInsensitive)||
        filetype.endsWith(".tiff", Qt::CaseInsensitive)||
        filetype.endsWith(".svg", Qt::CaseInsensitive)||
        filetype.endsWith(".avif", Qt::CaseInsensitive)){
        return true;
    }
    return false;
}

bool QuizzUI::isVideo(QString filetype)
{
    if(filetype.endsWith(".mp4", Qt::CaseInsensitive)||
        filetype.endsWith(".mov", Qt::CaseInsensitive)||
        filetype.endsWith(".gif", Qt::CaseInsensitive)||
        filetype.endsWith(".wmv", Qt::CaseInsensitive)||
        filetype.endsWith(".avi", Qt::CaseInsensitive)||
        filetype.endsWith(".mkv", Qt::CaseInsensitive)||
        filetype.endsWith(".flv", Qt::CaseInsensitive)||
        filetype.endsWith(".f4v", Qt::CaseInsensitive)){
        return true;
    }
    return false;
}

bool QuizzUI::isAudio(QString filetype)
{
    if(filetype.endsWith(".mp3", Qt::CaseInsensitive)||
        filetype.endsWith(".wav", Qt::CaseInsensitive)||
        filetype.endsWith(".aac", Qt::CaseInsensitive)||
        filetype.endsWith(".ogg", Qt::CaseInsensitive)||
        filetype.endsWith(".flac", Qt::CaseInsensitive)||
        filetype.endsWith(".m4a", Qt::CaseInsensitive)){
        return true;
    }
    return false;
}

QPushButton *QuizzUI::createMiniButton(QString buttonText)
{
    QPushButton* removeQuestionButton = new QPushButton(buttonText);
    removeQuestionButton->setFixedSize(30, 30);
    removeQuestionButton->setStyleSheet(
        "QPushButton { font-size: 18px; font-weight: bold; }"
    );
    return removeQuestionButton;
}

QVBoxLayout *QuizzUI::createMediaPlayer(QString mediaSource)
{
    QVBoxLayout* container = new QVBoxLayout();

    player = new QMediaPlayer();
    QAudioOutput *audioOutput = new QAudioOutput();

    player->setAudioOutput(audioOutput);
    player->setSource(QUrl::fromLocalFile(mediaSource));

    if(isVideo(mediaSource)){
        QVideoWidget *videoWidget = new QVideoWidget(this);
        player->setVideoOutput(videoWidget);
        videoWidget->setAspectRatioMode(Qt::KeepAspectRatio);
        videoWidget->setMinimumHeight(480);
        videoWidget->setMinimumWidth(854);
        container->addWidget(videoWidget);
    }


    QSlider *progressSlider = new QSlider(Qt::Horizontal, this);
    progressSlider->setRange(0, 1000);
    container->addWidget(progressSlider);

    connect(player, &QMediaPlayer::positionChanged, this, [=](qint64 position){
        if (!progressSlider->isSliderDown()) {
            qint64 duration = player->duration();
            if (duration > 0)
                progressSlider->setValue(position * 1000 / duration);
        }
    });

    connect(progressSlider, &QSlider::sliderMoved, this, [=](int value){
        qint64 duration = player->duration();
        player->setPosition(value * duration / 1000);
    });


    QHBoxLayout* playerContainer = new QHBoxLayout();

    QPushButton *restartButton = new QPushButton();
    restartButton->setFixedSize(30,30);
    restartButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    connect(restartButton, &QPushButton::clicked, this, [=](){
        player->setPosition(0);
    });
    playerContainer->addWidget(restartButton);
    playerContainer->addSpacerItem(new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QPushButton* playPauseButton = new QPushButton();
    playPauseButton->setFixedSize(30,30);
    playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(playPauseButton, &QPushButton::clicked, this, [=](){
        if(player->isPlaying()){
            player->pause();
            playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        }else{
            player->play();
            playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        }
    });
    playerContainer->addWidget(playPauseButton);
    playerContainer->addSpacerItem(new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QLabel* muteIcon = new QLabel();
    QPixmap pixMute = style()->standardIcon(QStyle::SP_MediaVolumeMuted).pixmap(32, 32);
    muteIcon->setPixmap(pixMute);
    playerContainer->addWidget(muteIcon);

    QSlider *volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setMaximumWidth(100);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(mediaVolume);
    audioOutput->setVolume(mediaVolume/100.0);
    connect(volumeSlider, &QSlider::valueChanged, this, [=](int value){
        audioOutput->setVolume(value / 100.0);
        mediaVolume = value;
    });
    playerContainer->addWidget(volumeSlider);

    QLabel* fullVolumeIcon = new QLabel();
    QPixmap pixFullVolume = style()->standardIcon(QStyle::SP_MediaVolume).pixmap(32, 32);
    fullVolumeIcon->setPixmap(pixFullVolume);
    playerContainer->addWidget(fullVolumeIcon);

    container->addLayout(playerContainer);

    return container;
}


