#include "WordlistDialog.h"
#include "ui_WordlistDialog.h"

#include <QMap>
#include <algorithm>


void WordListDialog::m_initGenerate(){
    ui->generateWordlist->setListName("Wordlist");
    ui->generateWordlist->setListModel(m_generateWordlistModel);

    ///
    /// placeholder texts
    ///
    ui->lineEditPermutes->setPlaceholderText("No# of permutations e.g. 2");

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

    ///
    /// setting validators...
    ///
    ui->lineEditPermutes->setValidator(new QIntValidator(1, 10, this));
    /* Range A-Z */
    ui->lineEditAZCombination->setValidator(new QIntValidator(1, 10, this));
    /* Range date */
    ui->lineEditDateFromDay->setValidator(new QIntValidator(1, 31, this));
    ui->lineEditDateFromMonth->setValidator(new QIntValidator(1, 12, this));
    ui->lineEditDateFromYear->setValidator(new QIntValidator(0, 3000, this));
    ui->lineEditDateToDay->setValidator(new QIntValidator(1, 31, this));
    ui->lineEditDateToMonth->setValidator(new QIntValidator(1, 12, this));
    ui->lineEditDateToYear->setValidator(new QIntValidator(0, 3000, this));

    ///
    /// setting default values...
    ///
    ui->lineEditAZCombination->setText("1");
}

void WordListDialog::on_buttonGenerate_clicked(){
    /* generate respective wordlist */
    if(ui->groupBoxAZ->isChecked())
        this->m_generateAZ();
    if(ui->groupBoxDate->isChecked())
        this->m_generateDate();
    if(ui->groupBoxNumbers->isChecked())
        this->m_generateNumbers();
    if(ui->groupBoxPermutations->isChecked())
        this->m_generatePermutations();
    if(ui->groupBoxSubstitutions->isChecked())
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

            /* if not exact combine count needed append the already generated combination */
            if(!ui->checkBoxExactCombination->isChecked())
                list.append(combinedList);

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

void WordListDialog::m_generatePermutations(){
    int numberOfPermuations = ui->lineEditPermutes->text().toInt();
    QStringList targets(m_generateWordlistModel->stringList());

    /* get the stringList from model */
    QStringList list(m_wordlistModel->stringList());

    do{
        /* creating the permutated string by appending */
        QString item("");
        for(int i = 0; i < numberOfPermuations; i++)
            item += targets.at(i);

        /* append the created string to the list */
        list.push_back(item);
    } while (std::next_permutation(targets.begin(), targets.end()));

    /* set the stringList to the model */
    m_wordlistModel->setStringList(list);
}

void WordListDialog::m_generateSubstitutions(){
    QMap<QString, QStringList> Homoglyphs{{"a",{"@","à","á","â","ã","ä","å"}},
                                          {"b",{"8","ß","ʙ","β","В","Ь","Ᏼ","ᛒ","Ｂ","ｂ"}},
                                          {"c",{"("}},
                                          {"d",{"Ď","ď","Đ","đ","ԁ","ժ"}},
                                          {"e",{"3","È","É" ,"Ê","Ë","é","ê","ë","Ē","ē","Ĕ","ĕ","Ė","ė","Ę","Ě","ě"}},
                                          {"f",{"7"}},
                                          {"g",{"q"}},
                                          {"h",{}},
                                          {"i",{"1","l","1"}},
                                          {"j",{}},
                                          {"k",{}},
                                          {"l",{}},
                                          {"m",{}},
                                          {"n",{}},
                                          {"o",{}},
                                          {"p",{}},
                                          {"q",{}},
                                          {"r",{}},
                                          {"s",{}},
                                          {"t",{}},
                                          {"u",{}},
                                          {"v",{}},
                                          {"w",{}},
                                          {"x",{}},
                                          {"y",{}},
                                          {"z",{}}};

    QMap<QString, QStringList> Substitutions{{"a",{"@"}},
                                          {"b",{"8"}},
                                          {"c",{"("}},
                                          {"d",{}},
                                          {"e",{"3"}},
                                          {"f",{"7"}},
                                          {"g",{"q"}},
                                          {"h",{}},
                                          {"i",{"1","l","1"}},
                                          {"j",{}},
                                          {"k",{}},
                                          {"l",{}},
                                          {"m",{}},
                                          {"n",{}},
                                          {"o",{}},
                                          {"p",{}},
                                          {"q",{}},
                                          {"r",{}},
                                          {"s",{}},
                                          {"t",{}},
                                          {"u",{}},
                                          {"v",{}},
                                          {"w",{}},
                                          {"x",{}},
                                          {"y",{}},
                                          {"z",{}}};
}
