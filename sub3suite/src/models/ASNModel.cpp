#include "ASNModel.h"


s3s_struct::ASN asn_model_to_struct(s3s_item::ASN *item){
    s3s_struct::ASN asn_struct;

    asn_struct.asn = item->text();

    asn_struct.info_asn = item->info_asn->text();
    asn_struct.info_name = item->info_name->text();
    asn_struct.info_description = item->info_description->text();
    asn_struct.info_country = item->info_country->text();
    asn_struct.info_website = item->info_website->text();
    asn_struct.info_ownerAddress = item->info_ownerAddress->text();

    asn_struct.rir_name = item->rir_name->text();
    asn_struct.rir_country = item->rir_country->text();
    asn_struct.rir_dateallocated = item->rir_dateAllocated->text();

    for(int i = 0; i < item->emailContacts->rowCount(); i++)
        asn_struct.emailcontacts.insert(item->emailContacts->child(i, 1)->text());

    for(int i = 0; i < item->abuseContacts->rowCount(); i++)
        asn_struct.abusecontacts.insert(item->abuseContacts->child(i, 1)->text());

    for(int i = 0; i < item->peers->rowCount(); i++)
        asn_struct.peers.insert(item->peers->child(i, 1)->text());

    for(int i = 0; i < item->prefixes->rowCount(); i++)
        asn_struct.prefixes.insert(item->prefixes->child(i, 1)->text());

    return asn_struct;
}

void asn_to_json(s3s_item::ASN *asn_item, QJsonObject &jsonObj){

}

void json_to_asn(const QJsonObject &jsonObj, s3s_item::ASN *asn_item){

}
