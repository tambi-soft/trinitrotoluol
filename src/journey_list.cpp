#include "journey_list.h"

JourneyList::JourneyList(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
}
