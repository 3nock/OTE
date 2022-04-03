/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "WordlistDialog.h"
#include "ui_WordlistDialog.h"

#include <QMap>
#include <QQueue>
#include <algorithm>

#include "src/utils/Config.h"


void WordListDialog::m_initGenerate(){
    ui->wordlist_generate->setListName("Wordlist");
    ui->wordlist_generate->setListModel(m_listModel_generate);

    /* Range A-Z */
    ui->lineEditAZCombination->setPlaceholderText("No# of combinations e.g. 2");
    /* Range date */
    ui->lineEditDateFromDay->setPlaceholderText("dd");
    ui->lineEditDateFromMonth->setPlaceholderText("mm");
    ui->lineEditDateFromYear->setPlaceholderText("yyyy");
    ui->lineEditDateToDay->setPlaceholderText("dd");
    ui->lineEditDateToMonth->setPlaceholderText("mm");
    ui->lineEditDateToYear->setPlaceholderText("yyyy");
    /* Range numbers */
    ui->lineEditNumbersFrom->setPlaceholderText("e.g. 1");
    ui->lineEditNumbersTo->setPlaceholderText("e.g. 100");
    ui->lineEditNumbersSkip->setPlaceholderText("e.g. 1");

    /* Range A-Z */
    ui->lineEditAZCombination->setValidator(new QIntValidator(1, 10, this));
    /* Range date */
    ui->lineEditDateFromDay->setValidator(new QIntValidator(1, 31, this));
    ui->lineEditDateFromMonth->setValidator(new QIntValidator(1, 12, this));
    ui->lineEditDateFromYear->setValidator(new QIntValidator(0, 3000, this));
    ui->lineEditDateToDay->setValidator(new QIntValidator(1, 31, this));
    ui->lineEditDateToMonth->setValidator(new QIntValidator(1, 12, this));
    ui->lineEditDateToYear->setValidator(new QIntValidator(0, 3000, this));

    ui->lineEditAZCombination->setText("1");

    /* setting the substitutes */
    CONFIG.beginGroup("wordlist_substitutions");
    QStringList keys(CONFIG.allKeys());
    foreach(const QString &key, keys){
        ui->comboBoxSubstituteFrom->addItem(key);
        ui->comboBoxSubstituteTo->addItem(CONFIG.value(key).toString());
    }
    CONFIG.endGroup();
}

void WordListDialog::on_buttonGenerate_clicked(){
    /* generate respective wordlist */
    if(ui->checkBoxAZ->isChecked())
        this->m_generateAZ();
    if(ui->checkBoxDates->isChecked())
        this->m_generateDate();
    if(ui->checkBoxNumbers->isChecked())
        this->m_generateNumbers();
    if(ui->checkBoxSubstitutions->isChecked())
        this->m_generateSubstitutions();

    /* end */
    emit wordlistLoaded();
    accept();
}

void WordListDialog::m_generateAZ(){
    int numberOfCombinations = ui->lineEditAZCombination->text().toInt();
    QStringList alphabet{"a","b","c","d","e","f","g","h","i","j","k","l","m",
                        "n","o","p","q","r","s","t","u","v","w","x","y","z"};

    /* get the stringList from model */
    QStringList list(m_wordlistModel->stringList());

    QStringList combinedList;
    for(int i = 0; i < numberOfCombinations; i++){
        if(i == 0)
            combinedList = alphabet;
        else
        {
            QStringList _combinedList;
            for(int j = 0; j < combinedList.length(); j++){
                foreach(const QString &letter, alphabet)
                    _combinedList.push_back(combinedList.at(j)+letter);
            }

            combinedList = _combinedList;
        }
    }

    /* append the created string to the list */
    list.append(combinedList);

    /* set the stringList to the model */
    m_wordlistModel->setStringList(list);
}

void WordListDialog::m_generateNumbers(){
    int from = ui->lineEditNumbersFrom->text().toInt();
    int to = ui->lineEditNumbersTo->text().toInt();
    int skip = ui->lineEditNumbersSkip->text().toInt();

    /* get the stringList from model */
    QStringList list(m_wordlistModel->stringList());

    for(int i = from; i <= to; i += skip)
        list.push_back(QString::number(i));

    /* set the stringList to the model */
    m_wordlistModel->setStringList(list);
}

/*
 * TODO:
 *      consider common years and leap years in generating dates
 *      for february month.
 */
void WordListDialog::m_generateDate(){
    /* getting input data */
    int fromDay = ui->lineEditDateFromDay->text().toInt();
    int fromMonth = ui->lineEditDateFromMonth->text().toInt();
    int fromYear = ui->lineEditDateFromYear->text().toInt();
    int toDay = ui->lineEditDateToDay->text().toInt();
    int toMonth = ui->lineEditDateToMonth->text().toInt();
    int toYear = ui->lineEditDateToYear->text().toInt();

    /* getting format the date */
    QString join = "";
    switch (ui->comboBoxDateFormat->currentIndex()) {
    case 0:
        join = ".";
        break;
    case 1:
        join = "-";
        break;
    case 2:
        join = "_";
        break;
    case 3:
        join = "";
    }

    /* get the stringList from model */
    QStringList list(m_wordlistModel->stringList());

    /* iterating through the date */
    for(int year = fromYear; year <= toYear; year++)
    {
        int _fromMonth = 1;
        int _toMonth = 12;
        if(year == toYear)
            _toMonth = toMonth;
        if(year == fromYear)
            _fromMonth = fromMonth;
        if((fromYear == toYear) && (fromMonth == toMonth)){
            _fromMonth = fromMonth;
            _toMonth = toMonth;
        }

        for(int month = _fromMonth; month <= _toMonth; month++)
        {
            int _fromDay = 1;

            /* get last date on month depending on the month */
            int _toDay = 31;
            switch (month) {
            case 2:  // february
                _toDay = 29;
                break;

            case 1:  // january
            case 3:  // march
            case 5:  // april
            case 7:  // july
            case 8:  // august
            case 10: // october
            case 12: // december
                _toDay = 31;
                break;

            case 4:  // april
            case 6:  // june
            case 9:  // september
            case 11: // november
                _toDay = 30;
                break;
            }

            /* ... */
            if((year == toYear) && (month == toMonth))
                _toDay = toDay;
            if((year == fromYear) && (month == fromMonth))
                _fromDay = fromDay;
            if((fromYear == toYear) && (fromMonth == toMonth)){
                _fromDay = fromDay;
                _toDay = toDay;
            }

            for(int day = _fromDay; day <= _toDay; day++)
                list.push_back(QString::number(day)+join+
                               QString::number(month)+join+
                               QString::number(year));
        }
    }

    /* set the stringList to the model */
    m_wordlistModel->setStringList(list);
}

/*
 * TODO:
 *      it is poorly implemented for now, later implement well.
 */
void WordListDialog::m_generateSubstitutions(){
    QMap<QString,QString> substitutions;
    QSet<QString> wordlist;


    /* obtaining the substitutions characters */
    for(int i = 0; i < ui->comboBoxSubstituteFrom->count(); i++)
        substitutions.insert(ui->comboBoxSubstituteFrom->itemText(i),
                             ui->comboBoxSubstituteTo->itemText(i));

    QStringList list(m_wordlistModel->stringList());

    /* substitutions */
    foreach(const QString &word, m_listModel_generate->stringList()){
        for(int i = 0; i < word.length(); i++)
        {
            QString character = word.at(i);
            if(substitutions.contains(character))
            {
                wordlist.insert(word);
                QString substitute(substitutions.value(character));

                QStringList new_list;
                QStringList setList = wordlist.toList();
                for(int j = 0; j < setList.size(); j++){
                    QString existing_word = setList.at(j);
                    new_list.append(existing_word.replace(i, substitute.length(), substitute));
                }
                foreach(const QString &word, new_list)
                    wordlist.insert(word);
            }
        }
        list.append(wordlist.toList());
        wordlist.clear();
    }

    /* set the stringList to the model */
    list.append(wordlist.toList());
    m_wordlistModel->setStringList(list);
}

void WordListDialog::on_comboBoxSubstituteFrom_currentIndexChanged(int index){
    ui->comboBoxSubstituteTo->setCurrentIndex(index);
}

void WordListDialog::on_comboBoxSubstituteTo_currentIndexChanged(int index){
    ui->comboBoxSubstituteFrom->setCurrentIndex(index);
}
