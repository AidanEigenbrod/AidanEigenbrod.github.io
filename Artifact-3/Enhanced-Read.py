import os
from pymongo import MongoClient
from bson.objectid import ObjectId


class AnimalShelter(object):

    def __init__(self):
        # Load credentials and connection details from environment variables
        user = os.getenv("MONGO_USERNAME")
        password = os.getenv("MONGO_PASSWORD")
        host = os.getenv("MONGO_HOST")
        port = os.getenv("MONGO_PORT")

        db_name = "AAC"
        col_name = "animals"

        try:
            
            port = int(port) if port else None

            uri = f"mongodb://{user}:{password}@{host}:{port}"
            self.client = MongoClient(uri)

            # Connect to the selected database and collection
            self.database = self.client[db_name]
            self.collection = self.database[col_name]

            print("Secure connection established successfully.")
        except Exception as e:
            print("Database connection failed:", e)


    def create(self, data):
        # Data must be a valid dictionary before inserting
        if isinstance(data, dict) and data:
            try:
                self.collection.insert_one(data)
                print("Record successfully added.")
            except Exception as e:
                print("Error inserting data:", e)
        else:
            raise Exception("Invalid or missing data.")


    def read(self, query=None):
        # If no query is provided, return all records
        try:
            if query is None:
                return list(self.collection.find())
            elif isinstance(query, dict):
                return list(self.collection.find(query))
            else:
                raise Exception("Invalid query format.")
        except Exception as e:
            print("Error reading from database:", e)


    def update(self, searchData, newData):
        # Both search criteria and new values are required for an update
        if searchData and newData:
            try:
                result = self.collection.update_many(searchData, {"$set": newData})
                return result.modified_count
            except Exception as e:
                print("Error updating database:", e)
        else:
            raise Exception("Missing search or update criteria.")


    def delete(self, searchData):
        # A valid search filter is required for deletion
        if searchData:
            try:
                result = self.collection.delete_many(searchData)
                return result.deleted_count
            except Exception as e:
                print("Error deleting data:", e)
        else:
            raise Exception("Missing delete criteria.")