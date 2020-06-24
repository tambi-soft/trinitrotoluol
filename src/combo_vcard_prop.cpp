#include "combo_vcard_prop.h"

ComboVCardProp::ComboVCardProp(QComboBox *parent) : QComboBox(parent)
{
    this->properties_standard << "[NONE]" << "ADR" << "ANNIVERSARY" << "BDAY" << "DEATHDATE" << "FN" << "EMAIL" << "GENDER" << "IMPP" << "GEO" << "NICKNAME" << "NOTE" << "TEL" << "LANG" << "Timezone" << "URL";
    
    this->properties_en << "[Choose Property Type]" << "Address" << "Anniversary" << "Birthday" << "Date of Death" << "Detailed Name" << "Email" << "Gender" << "Instant Messaging" << "Location" << "Nickname" << "Notes" << "Phone" << "Spoken Languages" << "Timezone" << "Website";
    
    addItems(this->properties_en);
    
    
    connect(this, qOverload<int>(&QComboBox::currentIndexChanged), this, &ComboVCardProp::vcardItemSelected);
}

void ComboVCardProp::vcardItemSelected(int index)
{
    emit vcardItemSelectedSignal(this->properties_standard[index]);
    
    blockSignals(true);
    setCurrentIndex(0);
    blockSignals(false);
}
