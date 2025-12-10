from pymongo import MongoClient
from bson.objectid import ObjectId


class AnimalShelter(object):
    def __init__(self, user = 'aacuser', password = 'AidEig2203!'): 
        HOST = 'nv-desktop-services.apporto.com'
        PORT = 30173
        DB = 'AAC'
        COL = 'animals'
        
        self.client = MongoClient('mongodb://%s:%s@%s:%d' % (user, password, HOST, PORT))
        self.database = self.client['%s' % (DB)]
        self.collection = self.database['%s' % (COL)]
        
    
    def create(self, data):
        if data is not None:
            self.collection.insert_one(data)
        else: 
            raise Exception("Nothing to save, because data parameter is empty")
            
    def read(self, dataSearch):
        if dataSearch is not None:
            return list(self.collection.find(dataSearch))
        else:
            return list(self.collection.find())
        
    def update(self, searchData, newData):
        if searchData and newData:
            modifiedData = self.collection.update_many(searchData, {"$set": newData})
            return modifiedData.modified_count
        else:
            raise Exception("Unable to locate and update information")
            
    def delete(self, searchData):
        if searchData:
            result = self.collection.delete_many(searchData)
            return result.deleted_count
        else:
            raise Exception("Unable to delete information")