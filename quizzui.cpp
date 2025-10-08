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
#include <QRandomGenerator>
#include <QGuiApplication>
#include <QScreen>
#include <QSettings>
#include <QFormLayout>

QuizzUI::QuizzUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QuizzUI)
{
    ui->setupUi(this);

    quiz = new Quiz();

    //Nutzen zum fertig builden
    //"C:\Qt\6.9.1\mingw_64\bin\windeployqt.exe" Quizapp.exe

    setWindowTitle("Quizapp");

    connect(ui->actionNeues_Quiz, &QAction::triggered, this, [=](){
        stopQuiz();
        if(quiz->getSavePath() != ""){
            quiz->save(quiz->getSavePath());
        }else{
            saveAs();
        }
        quiz = new Quiz();
        startEditing();
    });
    connect(ui->actionStartenStoppen, &QAction::triggered, this, [=](){
        if(quizStarted){
            stopQuiz();
        }else{
            startQuiz();
        }

    });
    connect(ui->actionSpeichern, &QAction::triggered, this, [=](){
        if(quiz->getSavePath() != ""){
            quiz->save(quiz->getSavePath());
        }else{
            saveAs();
        }
    });
    connect(ui->actionSpeichern_unter, &QAction::triggered, this, [=](){
        saveAs();
    });
    connect(ui->actionLaden, &QAction::triggered, this, [=](){
        stopQuiz();
        QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        QString fileName = QFileDialog::getExistingDirectory(
            this,
            "Quiz laden",
            documentsPath,
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
        quiz->load(fileName);
        startEditing();
    });

    QSettings settings("Yakoto","QuizApp");
    quiz->load(settings.value("lastEditedQuiz","").toString());
    mediaVolume = settings.value("MediaVolume",50).toInt();

    startEditing();
}

QuizzUI::~QuizzUI()
{
    delete ui;
}

void QuizzUI::saveAs()
{
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getExistingDirectory(
        this,
        "Quiz speichern",
        documentsPath,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    quiz->save(fileName);
}

void QuizzUI::showPlayerStats(QHBoxLayout *playerStatsContainer)
{
    clearLayout(playerStatsContainer);

    QVBoxLayout* descrContainer = new QVBoxLayout();
    descrContainer->addWidget(new QLabel());
    descrContainer->addWidget(new QLabel("Punkte:"));
    descrContainer->addWidget(new QLabel("Beantwortet:"));
    playerStatsContainer->addLayout(descrContainer);

    for(int i = 0; i < quiz->getPlayers().size();i++){
        Player* currPlayer = quiz->getPlayers().at(i);
        QWidget* playerStatsWidget = new QWidget();
        QVBoxLayout* playerStatsLayout = new QVBoxLayout();

        playerStatsLayout->addWidget(new QLabel(currPlayer->getName()), 0,Qt::AlignHCenter);

        QLabel* pointsLabel = new QLabel(QString::number(currPlayer->getPoints()));
        playerStatsLayout->addWidget(pointsLabel, 0,Qt::AlignHCenter);

        QLabel* correctAnswersLabel = new QLabel(QString::number(currPlayer->getCorrectAnswers()));
        playerStatsLayout->addWidget(correctAnswersLabel, 0,Qt::AlignHCenter);

        playerStatsWidget->setLayout(playerStatsLayout);
        if(quiz->getCurrPlayerIndex() == i){
            playerStatsWidget->setStyleSheet("QWidget { background-color: lightgrey; }");
        }
        playerStatsContainer->addWidget(playerStatsWidget);

    }

    addHSpacer(playerStatsContainer);
}
void QuizzUI::startQuiz()
{
    quizStarted = true;
    ui->actionStartenStoppen->setText("Stoppen");

    clearLayout(ui->categoryContainer);

    quiz->randomizePlayers();

    QHBoxLayout* categories = new QHBoxLayout();
    QHBoxLayout* playerStatsHorizontalLayout = new QHBoxLayout();

    for(int i = 0; i < quiz->getCategories().size();i++){
        Category* currCategory = quiz->getCategories().at(i);

        QVBoxLayout* container = new QVBoxLayout();
        QLabel* categoryName = new QLabel(currCategory->getName());
        categoryName->setWordWrap(true);
        categoryName->setStyleSheet("QLabel { font-size: 26px; }");
        container->addWidget(categoryName, 0,Qt::AlignHCenter);

        for(int j = 0; j < currCategory->getQuestions().size();j++){
            Question* currQuestion = currCategory->getQuestions().at(j);

            QPushButton* selectQuestionButton = new QPushButton(QString::number(currQuestion->getPoints()));
            selectQuestionButton->setStyleSheet("QPushButton { font-size: 26px; }");
            selectQuestionButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
            if(currQuestion->getAnswered()){
                selectQuestionButton->setDisabled(true);
            }
            connect(selectQuestionButton, &QPushButton::clicked, this, [=](){
                Question* currQuestion = quiz->getCategories().at(i)->getQuestions()[j];
                clearLayout(ui->categoryContainer);

                QVBoxLayout* questionContainer = new QVBoxLayout();
                ui->categoryContainer->addLayout(questionContainer);

                QLabel* questionDescr = new QLabel(currCategory->getName() + " "+ QString::number(currQuestion->getPoints()));
                questionContainer->addWidget(questionDescr, 0,Qt::AlignHCenter);

                QLabel* questionText = new QLabel(currQuestion->getQuestionText());
                questionText->setWordWrap(true);
                questionText->setProperty("class", "question");
                questionContainer->addWidget(questionText, 0, Qt::AlignHCenter);

                if (isImage(currQuestion->getContentSource()))
                {
                    QLabel* imageLabel = new QLabel("");
                    int screenHeight = QGuiApplication::primaryScreen()->size().height();
                    int screenWidth  = QGuiApplication::primaryScreen()->size().width();
                    QPixmap pix(currQuestion->getContentSource());
                    imageLabel->setPixmap(pix.scaled(QSize(screenWidth, screenHeight/2), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    questionContainer->addWidget(imageLabel, 0,Qt::AlignHCenter);
                }else if (isAudio(currQuestion->getContentSource())||isVideo(currQuestion->getContentSource())){
                    QVBoxLayout* mediaPlayerLayout = createMediaPlayer(currQuestion->getContentSource());

                    questionContainer->addLayout(mediaPlayerLayout);
                }

                if(currQuestion->getAnswer() != ""){
                    QLabel* answerText = new QLabel(currQuestion->getAnswer());
                    answerText->setWordWrap(true);
                    answerText->setVisible(false);
                    answerText->setProperty("class", "question");
                    answerText->setMinimumHeight(50);

                    questionContainer->addWidget(answerText, 0,Qt::AlignBottom | Qt::AlignHCenter);

                    QPushButton* revealAnserButton = new QPushButton("Antwort");
                    connect(revealAnserButton, &QPushButton::clicked, this, [=](){
                        revealAnserButton->setVisible(false);
                        answerText->setVisible(true);
                    });
                    revealAnserButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
                    revealAnserButton->setStyleSheet("QPushButton { padding: 4px 40px; }");
                    revealAnserButton->setMinimumHeight(50);

                    questionContainer->addWidget(revealAnserButton, 0,Qt::AlignBottom | Qt::AlignHCenter);
                }

                QHBoxLayout* playersContainer = new QHBoxLayout();
                for(int k = 0; k < quiz->getPlayers().size();k++){
                    Player* currPlayer = quiz->getPlayers().at(k);

                    QPushButton* setWinningPlayerButton = new QPushButton(currPlayer->getName());
                    connect(setWinningPlayerButton, &QPushButton::clicked, this, [=](){
                        currPlayer->addPoints(currQuestion->getPoints());
                        currPlayer->addCorrectAnswer();
                        currQuestion->setAnswered(true);
                        quiz->nextPlayer();
                        startQuiz();
                    });
                    playersContainer->addWidget(setWinningPlayerButton);
                }
                QPushButton* noWinnerButton = new QPushButton("Keiner");
                connect(noWinnerButton, &QPushButton::clicked, this, [=](){
                    currQuestion->setAnswered(true);
                    quiz->nextPlayer();
                    startQuiz();
                });
                playersContainer->addWidget(noWinnerButton);
                questionContainer->addLayout(playersContainer);

                QPushButton* cancelButton = new QPushButton("Abbrechen");
                cancelButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
                cancelButton->setStyleSheet("QPushButton { padding: 4px 40px; }");
                questionContainer->addWidget(cancelButton, 0,Qt::AlignHCenter);
                connect(cancelButton, &QPushButton::clicked, this, [=](){
                    startQuiz();
                });
            });

            container->addWidget(selectQuestionButton);
        }

        addVSpacer(container);

        categories->addLayout(container);
    }
    ui->categoryContainer->addLayout(categories);

    ui->categoryContainer->addLayout(playerStatsHorizontalLayout);
    showPlayerStats(playerStatsHorizontalLayout);
}

void QuizzUI::stopQuiz()
{
    quizStarted = false;
    ui->actionStartenStoppen->setText("Starten");

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
        categoryName->setWordWrap(true);
        categoryName->setStyleSheet("QLabel { font-size: 26px; }");
        QPushButton* removeCategoryButton = createMiniButton("X");
        removeCategoryButton->setStyleSheet("QPushButton { background-color: #ff7f7f; }");

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
            selectQuestionButton->setStyleSheet("QPushButton { font-size: 26px; }");
            selectQuestionButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
            connect(selectQuestionButton, &QPushButton::clicked, this, [=](){
                Question* questionToEdit = quiz->getCategories().at(i)->getQuestions()[j];

                questionEditingMenu(quiz->getCategories().at(i)->getQuestions()[j], i);

                startEditing();
            });
            questionContainer->addWidget(selectQuestionButton);

            QPushButton* removeQuestionButton = createMiniButton("X");
            removeQuestionButton->setStyleSheet("QPushButton { background-color: #ff7f7f; }");
            connect(removeQuestionButton, &QPushButton::clicked, this, [=](){
                quiz->getCategories().at(i)->removeQuestion(j);
                startEditing();
            });
            questionContainer->addWidget(removeQuestionButton);
            container->addLayout(questionContainer);
        }

        QPushButton* addQuestionButton = new QPushButton("Neue Frage");
        addQuestionButton->setStyleSheet("QPushButton { padding: 4px 40px; }");
        connect(addQuestionButton, &QPushButton::clicked, this, [=](){
            questionEditingMenu(nullptr, i);
            startEditing();
        });
        container->addWidget(addQuestionButton);

        addVSpacer(container);

        categories->addLayout(container);
    }

    QVBoxLayout* addCategoryContainer = new QVBoxLayout();
    QPushButton* addCategoryButton = new QPushButton("Neue Kategorie");
    addCategoryButton->setStyleSheet("QPushButton { padding: 4px 40px; }");
    connect(addCategoryButton, &QPushButton::clicked, this, [=](){
        addCategory();
    });
    addCategoryContainer->addWidget(addCategoryButton);
    addVSpacer(addCategoryContainer);
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

        playersContainer->addWidget(new QLabel(currPlayer->getName()), 0, Qt::AlignRight);
        QPushButton* removePlayerButton = createMiniButton("X");
        removePlayerButton->setStyleSheet("QPushButton { background-color: #ff7f7f; }");
        connect(removePlayerButton, &QPushButton::clicked, this, [=](){
            quiz->removePlayer(i);
            startEditing();
        });
        playersContainer->addWidget(removePlayerButton);
    }

    addHSpacer(playersContainer);

    ui->categoryContainer->addLayout(playersContainer);
}

void QuizzUI::questionEditingMenu(Question *questionToEdit, int categoryIndex)
{
    QWidget* questionOptions = new QWidget();
    questionOptions->setWindowTitle("Frage bearbeiten");
    questionOptions->setWindowModality(Qt::ApplicationModal);

    QVBoxLayout* container = new QVBoxLayout(questionOptions);

    QFormLayout *formLayout = new QFormLayout();
    container->addLayout(formLayout);

    QLineEdit* questionInput= new QLineEdit();
    questionInput->setText(questionToEdit?questionToEdit->getQuestionText():"");
    formLayout->addRow(new QLabel("Frage:"),questionInput);

    QLineEdit* answerInput= new QLineEdit();
    answerInput->setText(questionToEdit?questionToEdit->getAnswer():"");
    formLayout->addRow(new QLabel("Antwort:"), answerInput);

    QLineEdit* pointsInput= new QLineEdit();
    pointsInput->setText(QString::number(questionToEdit?questionToEdit->getPoints():0));
    formLayout->addRow(new QLabel("Punkte:"), pointsInput);


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
    //Pevent crsahing when deleting mediaplayer while running
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
        int screenHeight = QGuiApplication::primaryScreen()->size().height();
        int screenWidth  = QGuiApplication::primaryScreen()->size().width();
        videoWidget->setMaximumWidth(screenWidth);
        videoWidget->setMaximumHeight(screenHeight/2);
        videoWidget->setMinimumHeight(screenHeight/2);
        videoWidget->setMinimumWidth(screenWidth);
        container->addWidget(videoWidget);
    }

    createMediaProgressSlider(container);

    QHBoxLayout* playerContainer = new QHBoxLayout();

    QPushButton *restartButton = new QPushButton();
    restartButton->setFixedSize(30,30);
    restartButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    connect(restartButton, &QPushButton::clicked, this, [=](){
        player->setPosition(0);
    });
    playerContainer->addWidget(restartButton, 0,Qt::AlignLeft);

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
    playerContainer->addWidget(playPauseButton, 0,Qt::AlignCenter);

    addVolumeSlider(playerContainer,audioOutput);

    container->addLayout(playerContainer);

    return container;
}

void QuizzUI::createMediaProgressSlider(QBoxLayout *container)
{
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
}

void QuizzUI::addVolumeSlider(QBoxLayout *container, QAudioOutput* audioOutput)
{
    QWidget* columeSliderWidget = new QWidget();
    QHBoxLayout* volumeSliderContainer = new QHBoxLayout(columeSliderWidget);
    QLabel* muteIcon = new QLabel();
    QPixmap pixMute = style()->standardIcon(QStyle::SP_MediaVolumeMuted).pixmap(32, 32);
    muteIcon->setPixmap(pixMute);
    volumeSliderContainer->addWidget(muteIcon);

    QSlider *volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setMaximumWidth(100);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(mediaVolume);
    audioOutput->setVolume(mediaVolume/100.0);
    connect(volumeSlider, &QSlider::valueChanged, this, [=](int value){
        audioOutput->setVolume(value / 100.0);
        mediaVolume = value;
        QSettings settings("Yakoto","QuizApp");
        settings.setValue("MediaVolume",mediaVolume);
    });
    volumeSliderContainer->addWidget(volumeSlider, 0,Qt::AlignRight);

    QLabel* fullVolumeIcon = new QLabel();
    QPixmap pixFullVolume = style()->standardIcon(QStyle::SP_MediaVolume).pixmap(32, 32);
    fullVolumeIcon->setPixmap(pixFullVolume);
    volumeSliderContainer->addWidget(fullVolumeIcon);

    container->addWidget(columeSliderWidget, 0,Qt::AlignRight);
}

void QuizzUI::addHSpacer(QBoxLayout *container)
{
    container->addSpacerItem(new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
}

void QuizzUI::addVSpacer(QBoxLayout *container)
{
    container->addSpacerItem(new QSpacerItem( 40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
}
void QuizzUI::closeEvent(QCloseEvent *event)
{
    if(quiz->getSavePath() != ""){
        quiz->save(quiz->getSavePath());
    }else{
        saveAs();
    }

    event->accept(); // App darf beendet werden
}

