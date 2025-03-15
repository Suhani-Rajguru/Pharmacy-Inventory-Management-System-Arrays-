#include <stdio.h>
#include <string.h>

#define NAME_LEN 20
#define CONTACT_LEN 20
#define DB_SIZE 70
#define BBD_SIZE 20
#define SDB_SIZE 20
#define SUCCESS 1
#define FAILURE 0

struct ExpiryDate {
    int day;
    int month;
    int year;
};

struct Batch {
    char batchNumber[CONTACT_LEN];
    int quantityInStock;
    struct ExpiryDate Date;
    int totalSales;
};

struct Supplierinfo {
    int supplierID;
    char supplierName[NAME_LEN];
    char contactInfo[CONTACT_LEN];
    int quantitySupplied;
    int uniqueMedicationCount;
    int totalQuantitySupplied;
    float totalCostSupplied;
};

struct Medication {
    int medicationID;
    char medicationName[NAME_LEN];
    float pricePerUnit;
    struct Batch batches[BBD_SIZE];
    int batchCount;
    int reorderLevel;
    int totalQuantity;
    struct Supplierinfo suppliers[SDB_SIZE];
    int supplierCount;
};

void initialise_DB(struct Medication medications[], int size);
int addNewMedication(struct Medication medications[], int size);
void calculateTotalQuantityForAll(struct Medication medications[], int size); 
void calculateUniqueMedicationCount(struct Medication medications[], int size);
void findAll_rounderSupplier(struct Medication medications[], int medSize);
void calculateTotalCostSupplied(struct Medication medications[], int medCount);
void checkExpirationDates(struct Medication meds[], int medCount);
int deleteMedicationByID(struct Medication medications[],int size);
void findTop10LargestTurnover(struct Medication medications[], int medSize);
void searchMedication(struct Medication medications[], int size);
int sales_tracking(struct Medication medications[], int size);
void sort_medication_by_expiry_date(struct Medication medications[],int size);
void supplierManagement(struct Medication medications[], int size);
void stockAlert(struct Medication medications[],int size,int i);
int update(struct Medication medications[], int size);
int isDuplicate(struct Supplierinfo suppliers[], int supplierCount, int supplierID);
int addSupplierToMedication(struct Medication medications[], int size);
int searchSupplierInMedication(struct Medication medications[], int size);
int updateSupplierInMedication(struct Medication medications[], int size);


void initialise_DB(struct Medication medications[], int size) {
    for (int i = 0; i < size; i++) {
        medications[i].medicationID = 0; 
        medications[i].batchCount = 0; 
        medications[i].supplierCount = 0; 
        medications[i].totalQuantity = 0; 
        medications[i].reorderLevel = 0; 
        medications[i].pricePerUnit = 0.0f; 
        medications[i].medicationName[0] = '\0';

        // Initialize batches
        for (int j = 0; j < BBD_SIZE; j++) {
            medications[i].batches[j].batchNumber[0] = '\0'; 
            medications[i].batches[j].quantityInStock = 0; 
            medications[i].batches[j].totalSales = 0; 
            medications[i].batches[j].Date.day = 0; 
            medications[i].batches[j].Date.month = 0; 
            medications[i].batches[j].Date.year = 0; 
        }

        // Initialize suppliers
        for (int j = 0; j < SDB_SIZE; j++) {
            medications[i].suppliers[j].supplierID = 0; 
            medications[i].suppliers[j].quantitySupplied = 0;
            medications[i].suppliers[j].uniqueMedicationCount = 0; 
            medications[i].suppliers[j].totalQuantitySupplied = 0;
            medications[i].suppliers[j].totalCostSupplied = 0.0f; 
            medications[i].suppliers[j].supplierName[0] = '\0'; 
            medications[i].suppliers[j].contactInfo[0] = '\0'; 
        }
    }
}
 
int addNewMedication(struct Medication medications[], int size) 
{
    int idx = -1;
    int found = 0;

    for (int i = 0; i < DB_SIZE && (!found); i++) 
    {
        if (medications[i].medicationID == 0) 
        { 
            idx = i;
            found = 1;
        }
    }

    if (idx == -1) 
    {
        printf("Inventory full. Cannot add more medications.\n");
        return FAILURE;
    }

    printf("Enter Medication ID: ");
    scanf("%d", &medications[idx].medicationID);

    printf("Enter Medication Name: ");
    scanf(" %s", medications[idx].medicationName);

    printf("Enter Price Per Unit: ");
    scanf("%f", &medications[idx].pricePerUnit);

    printf("Enter Reorder Level: ");
    scanf("%d", &medications[idx].reorderLevel);

    printf("Enter number of batches: ");
    scanf("%d", &medications[idx].batchCount);

    for (int i = 0; i < medications[idx].batchCount; i++) 
    {
        printf("Enter Batch Number for Batch %d: ", i + 1);
        scanf(" %s", medications[idx].batches[i].batchNumber);

        printf("Enter Quantity in Stock for Batch %d: ", i + 1);
        scanf("%d", &medications[idx].batches[i].quantityInStock);

        printf("Enter Expiration Date (DD MM YYYY) for Batch %d: ", i + 1);
        scanf("%d %d %d", &medications[idx].batches[i].Date.day, 
                         &medications[idx].batches[i].Date.month, 
                         &medications[idx].batches[i].Date.year);
    }

    medications[idx].totalQuantity = 0;
    for (int i = 0; i < medications[idx].batchCount; i++) 
    {
        medications[idx].totalQuantity += medications[idx].batches[i].quantityInStock;
    }

    printf("Enter number of suppliers: ");
    scanf("%d", &medications[idx].supplierCount);

    for (int i = 0; i < medications[idx].supplierCount; i++) 
    {
        printf("Enter Supplier ID for Supplier %d: ", i + 1);
        scanf("%d", &medications[idx].suppliers[i].supplierID);

        printf("Enter Supplier Name for Supplier %d: ", i + 1);
        scanf(" %s", medications[idx].suppliers[i].supplierName);

        printf("Enter Contact Info for Supplier %d: ", i + 1);
        scanf(" %s", medications[idx].suppliers[i].contactInfo);

        printf("Enter Quantity Supplied by Supplier %d: ", i + 1);
        scanf("%d", &medications[idx].suppliers[i].quantitySupplied);

        medications[idx].suppliers[i].totalQuantitySupplied += medications[idx].suppliers[i].quantitySupplied;
    }

    printf("New medication added successfully at position %d.\n", idx + 1);
    return SUCCESS;
}
void calculateTotalQuantityForAll(struct Medication medications[], int size) 
{
    for (int i = 0; i < size; i++) 
    {   
        if (medications[i].medicationID != 0)
        {
        int totalQuantity = 0;
        for (int j = 0; j < medications[i].batchCount; j++) 
        {
            totalQuantity += medications[i].batches[j].quantityInStock;
        }
        medications[i].totalQuantity = totalQuantity;
        printf("total quantity of medication : %d is :%d",medications[i].medicationID,totalQuantity);
        }
    }
}


int compareExpiryDates(struct ExpiryDate date1, struct ExpiryDate date2) {
    if (date1.year != date2.year) {
        return (date1.year < date2.year) ? -1 : 1;
    } else if (date1.month != date2.month) {
        return (date1.month < date2.month) ? -1 : 1;
    } else {
        return (date1.day < date2.day) ? -1 : (date1.day > date2.day);
    }
}


int partition(struct Batch batches[], int low, int high, struct ExpiryDate pivot) {
    int i = low, j = high;
    struct Batch temp;

    while (i < j) {
        while (i < high && compareExpiryDates(batches[i].Date, pivot) <= 0) {
            i++;
        }
        while (compareExpiryDates(batches[j].Date, pivot) > 0) {
            j--;
        }
        if (i < j) {
            
            temp = batches[i];
            batches[i] = batches[j];
            batches[j] = temp;
        }
    }
    
    temp = batches[low];
    batches[low] = batches[j];
    batches[j] = temp;

    return j;
}


void quickSort(struct Batch batches[], int low, int high) 
{
    if (low < high) {
        struct ExpiryDate pivot = batches[low].Date;
        int partitionIndex = partition(batches, low, high, pivot);
        quickSort(batches, low, partitionIndex - 1);
        quickSort(batches, partitionIndex + 1, high);
    }
}

void sort_medication_by_expiry_date(struct Medication medications[],int size)
{
    for (int i = 0; i < size; i++) 
    {
        quickSort(medications[i].batches, 0, medications[i].batchCount - 1);
    }

}

int update(struct Medication medications[], int size) 
{
    int status_code = SUCCESS;
    int i = 0, found = 0;
    int id;
    printf("enter the id you want to update the details for : \n");
    scanf("%d",&id);
    
    // Search for the medication by ID
    while ((i < size) && (!found)) {
        if (id == medications[i].medicationID) {
            found = 1;
        } else {
            i++;
        }
    }

    if (found) {
        int option;
        float newPrice;
        int newReorderLevel;
        int newTotalQuantity;
        char newSupplierName[NAME_LEN];
        char newContactInfo[CONTACT_LEN];

        // Modify medication price
        printf("Enter 1 to modify price \n");   
        scanf("%d", &option);
        if (option == 1) {
            printf("Enter new price: ");
            scanf("%f", &newPrice);  
            medications[i].pricePerUnit = newPrice;
        }

        // Modify reorder level
        printf("Enter 2 to modify reorder level \n");
        scanf("%d", &option);
        if (option == 2) {
            printf("Enter new reorder level: ");
            scanf("%d", &newReorderLevel);
            medications[i].reorderLevel = newReorderLevel;
        }

        // Modify total quantity
        printf("Enter 3 to modify total quantity \n");
        scanf("%d", &option);
        if (option == 3) {
            printf("Enter new total quantity: ");
            scanf("%d", &newTotalQuantity);
            medications[i].totalQuantity = newTotalQuantity;
        }

        // Update supplier information
        printf("Enter 4 to modify supplier info \n");
        scanf("%d", &option);
        if (option == 4) {
            for (int j = 0; j < medications[i].supplierCount; j++) {
                printf("Enter updated ID: ");
                scanf("%d", &medications[i].suppliers[j].supplierID);
                
                printf("Enter updated name: ");
                scanf("%s", newSupplierName);
                strcpy(medications[i].suppliers[j].supplierName, newSupplierName);
                
                printf("Enter updated quantity supplied: ");
                scanf("%d", &medications[i].suppliers[j].totalQuantitySupplied);
                
                printf("Enter updated contact info: ");
                scanf("%s", newContactInfo);
                strcpy(medications[i].suppliers[j].contactInfo, newContactInfo);
            }
        }
        
    } else {
        printf("Medication with ID %d not found.\n", id);
        status_code = FAILURE;
    }

    return status_code;
}

int deleteMedicationByID(struct Medication medications[],int size) 
{   
    int idx;

    int found=0;
    for (int i = 0; i < DB_SIZE && (!found); i++) 
    {
        if (medications[i].medicationID == 0)
        { 
            idx = i;
            found=1;
        }
    }

    int medID, endPosition=idx;
    found = 0;

    printf("Enter Medication ID to delete: ");
    scanf("%d", &medID);

    for (int i = 0; i < endPosition; i++) 
    {
        if (medications[i].medicationID == medID) 
        {
            found = 1;
            printf("Medication found. Deleting...\n");

            for (int j = i; j < endPosition - 1; j++) 
            {
                medications[j] = medications[j + 1];
            }

            medications[endPosition - 1].medicationID = 0;
            medications[endPosition - 1].totalQuantity = 0;
            medications[endPosition - 1].batchCount = 0;
            medications[endPosition - 1].supplierCount = 0;
            medications[endPosition - 1].reorderLevel = 0;
            for (int k = 0; k < medications[endPosition - 1].batchCount; k++) 
            {
                 medications[endPosition - 1].batches[k].quantityInStock = 0; 
                 medications[endPosition - 1].batches[k].totalSales = 0;
                 medications[endPosition - 1].batches[k].batchNumber[0] = '\0'; 
                 medications[endPosition - 1].batches[k].Date.day = 0; 
                 medications[endPosition - 1].batches[k].Date.month = 0; 
                 medications[endPosition - 1].batches[k].Date.year = 0; 
            }

    
            for (int k = 0; k < medications[endPosition - 1].supplierCount; k++) 
            {
                 medications[endPosition - 1].suppliers[k].supplierID = 0; 
                 medications[endPosition - 1].suppliers[k].supplierName[0] = '\0'; 
                 medications[endPosition - 1].suppliers[k].contactInfo[0] = '\0'; 
                 medications[endPosition - 1].suppliers[k].quantitySupplied = 0; 
            }

            printf("Medication with ID %d deleted successfully.\n", medID);
            return SUCCESS;
        }
    }

    if (!found) 
    {
        printf("Medication with ID %d not found.\n", medID);
    }
    return FAILURE;
}

void searchMedication(struct Medication medications[], int size) {
    int Option;
    printf("enter 1 to search by Medication ID,\n enter 2 to search by Medication Name,\n enter 3 to search by Supplier ID ");
    printf("Enter your option: ");
    scanf("%d", &Option);

    switch (Option) {
        case 1: {
            int medID;
            printf("Enter Medication ID: ");
            scanf("%d", &medID);

            int found = 0;
            for (int i = 0; i < size&&(!found); i++) {
                if (medications[i].medicationID == medID) 
                {
                    printf("Medication found:\n");
                    printf("ID: %d\nName: %s,\nPrice per Unit: %f,\nReorder Level: %d,\n",
                           medications[i].medicationID, medications[i].medicationName,
                           medications[i].pricePerUnit, medications[i].reorderLevel);
                    printf("batches of medicine and quantity in stock according to batch no.");
                    for (int j=0;j<medications[i].batchCount;j++)
                    {
                        printf("quantity in stock of medicine in batch no. :%s is %d\n",medications[i].batches[j].batchNumber,medications[i].batches[j].quantityInStock);
                    }
                    found = 1;
                }  
            }
            if (!found) {
                printf("Medication with ID %d not found.\n", medID);
            }
            break;
        }
        case 2: {
            
            char medName[NAME_LEN];
            printf("Enter Medication Name: ");
            scanf("%s", medName);

            int found = 0;
            for (int i = 0; i < size; i++) 
            {
                if (strcmp(medications[i].medicationName, medName) == 0) 
                {
                    printf("Medication found.\n");
                    printf("ID: %d,\nName: %s,\nPrice per Unit: %f,\nReorder Level: %d,\n",
                           medications[i].medicationID, medications[i].medicationName, 
                           medications[i].pricePerUnit,  medications[i].reorderLevel);
                    printf("batches of medicine and quantity in stock according to batch no.");
                    for(int j=0;j<medications[i].batchCount;j++)
                    {
                        printf("quantity in stock of medicine in batchno :%s is %d",medications[i].batches[j].batchNumber,medications[i].batches[j].quantityInStock);
                    }
                    
                    found = 1;
                }
            }
            if (!found)
            {
                printf("No medications found with the name %s.\n", medName);
            }
            break;
        }
        case 3: {
            int suppID;
            printf("Enter Supplier ID: ");
            scanf("%d",&suppID);

            int found = 0;
            for (int i = 0; i < size ; i++) {
                for (int j = 0; j < medications[i].supplierCount; j++) 
                {
                    if (medications[i].suppliers[j].supplierID==suppID) 
                    {
                        printf("Supplier found: %d\n", medications[i].suppliers[j].supplierID);
                        printf("Medication Name: %s,\nPrice per Unit: %f,Reorder Level: %d,\n",
                               medications[i].medicationName,  medications[i].pricePerUnit, medications[i].reorderLevel);
                        found = 1;
                    }
                }
            }
            if (!found) {
                printf("No medications found for supplier %d.\n", suppID);
            }
            break;
        }
        default:
            printf("Invalid search option!\n");
            break;
    }
}

int daysInMonth(int month) {
    int daysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return daysPerMonth[month - 1];
}


int calculateDateDifference(struct ExpiryDate date1, struct ExpiryDate date2) {
    int days1 = 0, days2 = 0;
    
    
    days1 = date1.year * 365 + date1.day;
    for (int i = 1; i < date1.month; i++) {
        days1 += daysInMonth(i);
    }
    
   
    days2 = date2.year * 365 + date2.day;
    for (int i = 1; i < date2.month; i++) {
        days2 += daysInMonth(i);
    }
    
    
    return days2 - days1;
}


void checkExpirationDates(struct Medication meds[], int medCount) {
    struct ExpiryDate today;
    int i, j;

    
    printf("Enter today's date (DD MM YYYY): ");
    scanf("%d %d %d", &today.day, &today.month, &today.year);

    
    for (i = 0; i < medCount; i++) {
        for (j = 0; j < meds[i].batchCount; j++) {
            struct ExpiryDate batchExpiry = meds[i].batches[j].Date;
            int daysToExpiry = calculateDateDifference(today, batchExpiry);
            
            if (daysToExpiry > 0 && daysToExpiry <= 30) {
                
                printf("ALERT: Medication ID %d (Batch %s) is expiring in %d days!\n",
                       meds[i].medicationID, meds[i].batches[j].batchNumber, daysToExpiry);
            } else if (daysToExpiry <= 0) {
                
                printf("EXPIRED: Medication ID %d (Batch %s) has already expired!\n",
                       meds[i].medicationID, meds[i].batches[j].batchNumber);
            }
        }
    }
}

void stockAlert(struct Medication medications[],int size,int i)
{
    printf("checking stock levels....\n");
    if(medications[i].totalQuantity<=medications[i].reorderLevel)
    {
        printf("Alert : Medication '% s'(ID:%d)needs restocking! \n",medications[i].medicationName,medications[i].medicationID);
        printf("current stock : %d,\n Reorder Level : %d\n",medications[i].totalQuantity,medications[i].reorderLevel);
    }
    else
    {
        printf("Quantity in stock is sufficient");
    }
}

int sales_tracking(struct Medication medications[], int size) 
{   
    int quantity,medID;
    printf("enter the id of medicine you want to buy: \n");
    scanf("%d",&medID);
    printf("enter the quantity: \n");
    scanf("%d",& quantity);
    int i, j;
    int remainingQty = quantity;  
    int batchQty = 0;
    int found=0;             
 
    for (i = 0; (i < size)&&(!found); i++) {
        if (medications[i].medicationID == medID) 
        {
            printf("Medication found:%d\n", medications[i].medicationID);
            found=1;

           
            for (j = 0; j < medications[i].batchCount; j++) {
                batchQty = medications[i].batches[j].quantityInStock; 

                if (remainingQty <= batchQty) {
                   
                    medications[i].batches[j].quantityInStock -= remainingQty;
                    medications[i].batches[j].totalSales += remainingQty;
                    medications[i].totalQuantity-=remainingQty;
                    printf("Sale completed for %d units from batch %s.\n", 
                           remainingQty, medications[i].batches[j].batchNumber);
                    printf("now total quantity of id :%d is :%d \n",medications[i].medicationID,medications[i].totalQuantity);        
                    stockAlert(medications,DB_SIZE,i);
                    return SUCCESS;
                }
                else 
                {
                    
                    printf("Depleting batch %s: %d units sold.\n", 
                           medications[i].batches[j].batchNumber, batchQty);
                    remainingQty -= batchQty;  
                    medications[i].batches[j].totalSales += batchQty;
                    medications[i].batches[j].quantityInStock = 0; 
                    medications[i].totalQuantity-=batchQty;
                     printf("now total quantity of id :%d is :%d \n",medications[i].medicationID,medications[i].totalQuantity); 
                     stockAlert(medications,DB_SIZE,i);
                }
            }
            if (remainingQty > 0) 
            {
                printf("Error: Not enough stock to fulfill %d units. Only partial order fulfilled.\n", quantity);
                return FAILURE;
            }
        }
    }

    
    printf("Error: Medication with ID %d not found.\n", medID);
    return FAILURE;
}

//add
int addSupplierToMedication(struct Medication medications[], int size) 
{
    int medID, supplierIndex;
    printf("Enter Medication ID to add a supplier: ");
    scanf("%d", &medID);
    int found = 0;
    int medIndex = -1;
    for (int i = 0; i < size && (!found); i++) 
    {
        if (medications[i].medicationID == medID) 
        {
            medIndex = i;
            found = 1;
        }
    }
    if (medIndex == -1) 
    {
        printf("Medication not found.\n");
        return FAILURE;
    }

    if (medications[medIndex].supplierCount < SDB_SIZE) 
    {
        supplierIndex = medications[medIndex].supplierCount;

        printf("Enter Supplier ID: ");
        scanf("%d", &medications[medIndex].suppliers[supplierIndex].supplierID);
        printf("Enter Supplier Name: ");
        scanf("%s", medications[medIndex].suppliers[supplierIndex].supplierName);
        printf("Enter Supplier Contact Info: ");
        scanf("%s", medications[medIndex].suppliers[supplierIndex].contactInfo);
        printf("Enter Quantity Supplied: ");
        scanf("%d", &medications[medIndex].suppliers[supplierIndex].quantitySupplied);
        medications[medIndex].supplierCount++;
        printf("Supplier added successfully to Medication ID: %d.\n", medID);
        return SUCCESS;
    } 
    else 
    {
        printf("No more suppliers can be added for this medication.\n");
        return FAILURE;
    }
}

//update
int updateSupplierInMedication(struct Medication medications[], int size) 
{
    int medID, supplierID;
    printf("Enter Medication ID to update a supplier: ");
    scanf("%d", &medID);

    int medIndex = -1;
    int found = 0;
    for (int i = 0; i < size && (!found); i++) 
    {
        if (medications[i].medicationID == medID) 
        {
            medIndex = i;
            found = 1;
        }
    }
    if (medIndex == -1) 
    {
        printf("Medication not found.\n");
        return FAILURE;
    }

    printf("Enter Supplier ID to update: ");
    scanf("%d", &supplierID);

    int supplierIndex = -1;
    int found1=0;
    for (int j = 0; j < medications[medIndex].supplierCount && (!found1); j++) 
    {
        if (medications[medIndex].suppliers[j].supplierID == supplierID) 
        {
            supplierIndex = j;
            found1= 1;
        }
    }
    if (supplierIndex == -1) 
    {
        printf("Supplier not found.\n");
        return FAILURE;
    }

    printf("Enter updated Supplier Name: ");
    scanf("%s", medications[medIndex].suppliers[supplierIndex].supplierName);
    printf("Enter updated Contact Info: ");
    scanf("%s", medications[medIndex].suppliers[supplierIndex].contactInfo);
    printf("Enter updated Quantity Supplied: ");
    scanf("%d", &medications[medIndex].suppliers[supplierIndex].quantitySupplied);
    printf("Supplier updated successfully for Medication ID: %d.\n", medID);
    return SUCCESS;
}


//search
int searchSupplierInMedication(struct Medication medications[], int size) 
{
    int medID, supplierID;
    printf("Enter Medication ID to search for a supplier: ");
    scanf("%d", &medID);

    int medIndex = -1;
    int found = 0;
    for (int i = 0; i < size && (!found); i++) 
    {
        if (medications[i].medicationID == medID) 
        {
            medIndex = i;
            found = 1;
        }
    }
    if (medIndex == -1) 
    {
        printf("Medication not found.\n");
        return FAILURE;
    }

    printf("Enter Supplier ID to search: ");
    scanf("%d", &supplierID);

    int supplierIndex = -1;
    int found1 = 0;
    for (int j = 0; j < medications[medIndex].supplierCount && (!found1); j++) 
    {
        if (medications[medIndex].suppliers[j].supplierID == supplierID) 
        {
            supplierIndex = j;
            found1 = 1;
        }
    }
    if (supplierIndex == -1) 
    {
        printf("Supplier not found.\n");
        return FAILURE;
    }

    printf("Supplier ID: %d\n", medications[medIndex].suppliers[supplierIndex].supplierID);
    printf("Supplier Name: %s\n", medications[medIndex].suppliers[supplierIndex].supplierName);
    printf("Contact Info: %s\n", medications[medIndex].suppliers[supplierIndex].contactInfo);
    printf("Quantity Supplied: %d\n", medications[medIndex].suppliers[supplierIndex].quantitySupplied);

    return SUCCESS;
}

void supplierManagement(struct Medication medications[], int size) 
{
    int choice;
    do {
        printf("Supplier Management Menu:\n");
        printf("1. Add Supplier\n");
        printf("2. Update Supplier\n");
        printf("3. Search Supplier\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addSupplierToMedication(medications, size);
                break;
            case 2:
                updateSupplierInMedication(medications, size);
                break;
            case 3:
                searchSupplierInMedication(medications, size);
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 4);
}
void calculateUniqueMedicationCount(struct Medication medications[], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < medications[i].supplierCount; j++) {
            medications[i].suppliers[j].uniqueMedicationCount = 0; 
        }
    }
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < medications[i].supplierCount; j++) {
            int supplierID = medications[i].suppliers[j].supplierID;
            int uniqueMedCount = 0;
            for (int k = 0; k < size; k++) {
                int found = 0;
                for (int l = 0; l < medications[k].supplierCount && !found; l++) {
                    if (medications[k].suppliers[l].supplierID == supplierID) {
                        uniqueMedCount++;
                        found = 1;
                    }
                }
            }
            medications[i].suppliers[j].uniqueMedicationCount = uniqueMedCount;
            printf("unique count for supplier : %d is : %d",supplierID,uniqueMedCount);
        }
    }
}
int isDuplicate(struct Supplierinfo suppliers[], int supplierCount, int supplierID) {
    for (int i = 0; i < supplierCount; i++) {
        if (suppliers[i].supplierID == supplierID) {
            return 1; 
        }
    }
    return 0; 
}

int partition2(struct Supplierinfo suppliers[], int lo, int hi) {
    int pivot = suppliers[lo].uniqueMedicationCount;
    int i = lo + 1;
    int j = hi;

    while (i <= j) {
        if (suppliers[i].uniqueMedicationCount >= pivot) {
            i++;
        } else if (suppliers[j].uniqueMedicationCount < pivot) {
            j--;
        } else {
            struct Supplierinfo temp = suppliers[i];
            suppliers[i] = suppliers[j];
            suppliers[j] = temp;
            i++;
            j--;
        }
    }
    struct Supplierinfo temp = suppliers[j];
    suppliers[j] = suppliers[lo];
    suppliers[lo] = temp;

    return j;
}
void quickSort2(struct Supplierinfo suppliers[], int lo, int hi) {
    if (lo < hi) {
        int p = partition2(suppliers, lo, hi);
        quickSort2(suppliers, lo, p - 1);
        quickSort2(suppliers, p + 1, hi);
    }
}
void findAll_rounderSupplier(struct Medication medications[], int medSize) {
    struct Supplierinfo allSuppliers[DB_SIZE * SDB_SIZE];
    int supplierIdx = 0;
    for (int i = 0; i < medSize; i++) {
        for (int j = 0; j < medications[i].supplierCount; j++) {
            if (!isDuplicate(allSuppliers, supplierIdx, medications[i].suppliers[j].supplierID)) {
                allSuppliers[supplierIdx] = medications[i].suppliers[j];
                supplierIdx++;
            }
        }
    }
    
    quickSort2(allSuppliers, 0, supplierIdx - 1);
    printf("Top All-Rounder Suppliers (Top 10 by Unique Medication Count):\n");

    int limit = supplierIdx < 10 ? supplierIdx : 10;
    for (int i = 0; i < limit; i++) {
        printf("%d. Supplier ID: %d, Name: %s, Unique Medication Count: %d\n",
               i + 1, allSuppliers[i].supplierID, allSuppliers[i].supplierName,
               allSuppliers[i].uniqueMedicationCount);
    }

    
    if (supplierIdx > 0) {
        printf("\nBest All-Rounder Supplier:\n");
        printf("Supplier ID: %d, Name: %s, Unique Medication Count: %d\n",
               allSuppliers[0].supplierID, allSuppliers[0].supplierName,
               allSuppliers[0].uniqueMedicationCount);
    }
}

void calculateTotalCostSupplied(struct Medication medications[], int medCount) {
    for (int i = 0; i < medCount; i++) {
        for (int j = 0; j < medications[i].supplierCount; j++) {
            medications[i].suppliers[j].totalCostSupplied = 0.0; 
        }
    }
    for (int i = 0; i < medCount; i++) {
        for (int j = 0; j < medications[i].supplierCount; j++) {
            int supplierID = medications[i].suppliers[j].supplierID;
            float totalCost = 0.0;

            
            for (int k = 0; k < medCount; k++) {
                int found=0;
                for (int l = 0; l < medications[k].supplierCount && (!found); l++) {
                    if (medications[k].suppliers[l].supplierID == supplierID) {
                        totalCost += medications[k].pricePerUnit * medications[k].suppliers[l].quantitySupplied;
                        found=1;
                    }
                }
            }
           
            medications[i].suppliers[j].totalCostSupplied = totalCost;
        }
    }
}

int partition1(struct Supplierinfo suppliers[], int lo, int hi) {
    float pivot = suppliers[lo].totalCostSupplied; 
    int i = lo +1;
    int j=hi;  
    while (i <= j) {
        if (suppliers[i].totalCostSupplied >= pivot) {
            i++;
        } else if (suppliers[j]. totalCostSupplied< pivot) {
            j--;
        } else {
            struct Supplierinfo temp = suppliers[i];
            suppliers[i] = suppliers[j];
            suppliers[j] = temp;
            i++;
            j--;
        }
    }
    
    struct Supplierinfo temp = suppliers[j];
    suppliers[j] = suppliers[lo];
    suppliers[lo] = temp;

    return j;
}


void quickSort1(struct Supplierinfo suppliers[], int lo, int hi) {
    if (lo < hi) {
      
        int p = partition1(suppliers, lo, hi);

        quickSort1(suppliers, lo, p - 1);
        quickSort1(suppliers, p + 1, hi);
    }
}


void findTop10LargestTurnover(struct Medication medications[], int medSize) {
    struct Supplierinfo allSuppliers[DB_SIZE * SDB_SIZE];
    int supplierIdx = 0;
    for (int i = 0; i < medSize; i++) {
        for (int j = 0; j < medications[i].supplierCount; j++) {
            if (!isDuplicate(allSuppliers, supplierIdx, medications[i].suppliers[j].supplierID)) {
                allSuppliers[supplierIdx] = medications[i].suppliers[j];
                supplierIdx++;
            }
        }
    }
    quickSort1(allSuppliers, 0, supplierIdx - 1);
     printf("Top 10 Largest Turnover Suppliers are:\n");

    int limit = supplierIdx < 10 ? supplierIdx : 10;
    for (int i = 0; i < limit; i++) {
        printf("%d. Supplier ID: %d, Name: %s, total cost supplied : %f\n",
               i + 1, allSuppliers[i].supplierID, allSuppliers[i].supplierName,
               allSuppliers[i].totalCostSupplied);
    }

    
    if (supplierIdx > 0) {
        printf("\nBest largest turnover supplier:\n");
        printf("Supplier ID: %d, Name: %s,total cost supplied : %f\n",
               allSuppliers[0].supplierID, allSuppliers[0].supplierName,
               allSuppliers[0].totalCostSupplied);
    }
    
    
}


int main()
{
    struct Medication medications[DB_SIZE];
    struct Supplierinfo suppliers[SDB_SIZE];
    struct Batch batches[BBD_SIZE];
    struct ExpiryDate Date;
    initialise_DB(medications, DB_SIZE);
    medications[0].medicationID = 101;
strcpy(medications[0].medicationName, "Aspirin");
medications[0].pricePerUnit = 2.5;
medications[0].reorderLevel = 50;
medications[0].batchCount = 2;

strcpy(medications[0].batches[0].batchNumber, "BATCH001");
medications[0].batches[0].quantityInStock = 150;
medications[0].batches[0].Date = (struct ExpiryDate){15, 3, 2025};
medications[0].batches[0].totalSales = 50;

strcpy(medications[0].batches[1].batchNumber, "BATCH002");
medications[0].batches[1].quantityInStock = 100;
medications[0].batches[1].Date = (struct ExpiryDate){10, 5, 2025};
medications[0].batches[1].totalSales = 40;

medications[0].totalQuantity = 150 + 100;
medications[0].supplierCount = 1;
medications[0].suppliers[0] = (struct Supplierinfo){1, "SupplierOne", "123-456", 150};  

medications[1].medicationID = 102;
strcpy(medications[1].medicationName, "Aspirin");
medications[1].pricePerUnit = 3.0;
medications[1].reorderLevel = 30;
medications[1].batchCount = 2;

strcpy(medications[1].batches[0].batchNumber, "BATCH001");  
medications[1].batches[0].quantityInStock = 150;
medications[1].batches[0].Date = (struct ExpiryDate){12, 3, 2025};
medications[1].batches[0].totalSales = 70;

strcpy(medications[1].batches[1].batchNumber, "BATCH003");
medications[1].batches[1].quantityInStock = 200;
medications[1].batches[1].Date = (struct ExpiryDate){25, 6, 2025};
medications[1].batches[1].totalSales = 80;

medications[1].totalQuantity = 150 + 200;

medications[1].supplierCount = 2;
medications[1].suppliers[0] = (struct Supplierinfo){3, "SupplierThree", "234-567", 200}; 
medications[1].suppliers[1] = (struct Supplierinfo){2, "SupplierTwo", "789-012", 100};  


medications[2].medicationID = 103;
strcpy(medications[2].medicationName, "Paracetamol");
medications[2].pricePerUnit = 1.5;
medications[2].reorderLevel = 40;
medications[2].batchCount = 2;

strcpy(medications[2].batches[0].batchNumber, "BATCH002");  
medications[2].batches[0].quantityInStock = 100;
medications[2].batches[0].Date = (struct ExpiryDate){10, 5, 2025};
medications[2].batches[0].totalSales = 30;

strcpy(medications[2].batches[1].batchNumber, "BATCH004");
medications[2].batches[1].quantityInStock = 120;
medications[2].batches[1].Date = (struct ExpiryDate){22, 9, 2024};
medications[2].batches[1].totalSales = 50;

medications[2].totalQuantity = 100 + 120;


medications[2].supplierCount = 2;
medications[2].suppliers[0] = (struct Supplierinfo){2, "SupplierTwo", "789-012", 100};  
medications[2].suppliers[1] = (struct Supplierinfo){1, "SupplierOne", "123-456", 120};  


medications[3].medicationID = 104;
strcpy(medications[3].medicationName, "Amoxicillin");
medications[3].pricePerUnit = 4.0;
medications[3].reorderLevel = 20;
medications[3].batchCount = 2;

strcpy(medications[3].batches[0].batchNumber, "BATCH003"); 
medications[3].batches[0].quantityInStock = 100;
medications[3].batches[0].Date = (struct ExpiryDate){25, 6, 2025};
medications[3].batches[0].totalSales = 50;

strcpy(medications[3].batches[1].batchNumber, "BATCH005");
medications[3].batches[1].quantityInStock = 60;
medications[3].batches[1].Date = (struct ExpiryDate){5, 2, 2024};
medications[3].batches[1].totalSales = 20;

medications[3].totalQuantity = 100 + 60;


medications[3].supplierCount = 1;
medications[3].suppliers[0] = (struct Supplierinfo){3, "SupplierThree", "234-567", 160};  


medications[4].medicationID = 105;
strcpy(medications[4].medicationName, "Cetirizine");
medications[4].pricePerUnit = 2.0;
medications[4].reorderLevel = 10;
medications[4].batchCount = 2;

strcpy(medications[4].batches[0].batchNumber, "BATCH004");  
medications[4].batches[0].quantityInStock = 120;
medications[4].batches[0].Date = (struct ExpiryDate){22, 9, 2024};
medications[4].batches[0].totalSales = 60;

strcpy(medications[4].batches[1].batchNumber, "BATCH006");
medications[4].batches[1].quantityInStock = 80;
medications[4].batches[1].Date = (struct ExpiryDate){30, 6, 2024};
medications[4].batches[1].totalSales = 30;

medications[4].totalQuantity = 120 + 80;


medications[4].supplierCount = 2;
medications[4].suppliers[0] = (struct Supplierinfo){2, "SupplierTwo", "789-012" ,200}; 
medications[4].suppliers[1] = (struct Supplierinfo){3, "SupplierThree", "234-567", 100};


medications[5].medicationID = 106;
strcpy(medications[5].medicationName, "Metformin");
medications[5].pricePerUnit = 1.0;
medications[5].reorderLevel = 25;
medications[5].batchCount = 2;

strcpy(medications[5].batches[0].batchNumber, "BATCH005");  
medications[5].batches[0].quantityInStock = 60;
medications[5].batches[0].Date = (struct ExpiryDate){5, 2, 2024};
medications[5].batches[0].totalSales = 30;

strcpy(medications[5].batches[1].batchNumber, "BATCH007");
medications[5].batches[1].quantityInStock = 100;
medications[5].batches[1].Date = (struct ExpiryDate){8, 9, 2025};
medications[5].batches[1].totalSales = 50;

medications[5].totalQuantity = 60 + 100;


medications[5].supplierCount = 1;
medications[5].suppliers[0] = (struct Supplierinfo){2, "SupplierTwo", "789-012" , 160}; 

    calculateTotalQuantityForAll(medications,DB_SIZE);
    printf("\n");
    sort_medication_by_expiry_date(medications,DB_SIZE);

    int choice;

   
    printf("Welcome to the Pharmacy Inventory Management System.\n");

    do {
        printf("\n----------------------------------------\n");
        printf("Main Menu:\n");
        printf("1. Add New Medication: Add new medications with batch and supplier details.\n");
        printf("2. Update Medication Details: Modify the details of an existing medication.\n");
        printf("3. Delete Medication: Remove a medication from the inventory.\n");
        printf("4. Search Medication: Look up medications by ID, name, or supplier.\n");
        printf("5. Check Expiration Dates: View and manage medications nearing expiry.\n");
        printf("6. Sales Tracking: Record sales and update inventory levels accordingly.\n");
        printf("7. Supplier Management: Add, update, or search for supplier information.\n");
        printf("8. Find All-rounder Suppliers: Identify suppliers offering the most unique medications.\n");
        printf("9. Find Suppliers with Largest Turnover: Identify suppliers with the highest turnover.\n");
        printf("0. Exit: Quit the system.\n");
        printf("----------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("\nYou chose to add a new medication.\n");
                addNewMedication(medications,DB_SIZE);
                break;
            case 2:
                printf("\nYou chose to update existing medication details.\n");
                update(medications,DB_SIZE);
                break;
            case 3:
                printf("\nYou chose to delete a medication.\n");
                deleteMedicationByID(medications,DB_SIZE);
                break;
            case 4:
                printf("\nYou chose to search for a medication.\n");
                searchMedication(medications,DB_SIZE); 
                break;
            case 5:
                printf("\nChecking expiration dates of medications...\n");
                checkExpirationDates(medications, DB_SIZE);
                break;
            case 6:
                printf("\nYou chose to track sales and update inventory.\n");
                sales_tracking(medications, DB_SIZE);
                break;
            case 7:
                printf("\nManaging supplier information...\n");
                supplierManagement(medications,DB_SIZE);
                break;
            case 8:
                printf("\nFinding top all-rounder suppliers...\n");
                calculateUniqueMedicationCount(medications, DB_SIZE);
                findAll_rounderSupplier(medications,DB_SIZE);
                break;
            case 9:
                printf("\nFinding suppliers with the largest turnover...\n");
                calculateTotalCostSupplied(medications, DB_SIZE);
                findTop10LargestTurnover(medications, DB_SIZE);
                break;
            case 0:
                printf("\nExiting the system. Thank you for using the Pharmacy Inventory Management System. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Please enter a number between 0 and 9.\n");
        }
    } while (choice != 0);

    return 0;
}