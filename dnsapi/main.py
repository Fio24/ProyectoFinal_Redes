import base64
import dns.query
import csv
import ipaddress
import time
from flask_cors import CORS

from flask import Flask, jsonify, request
import json
import firebase_admin
from firebase_admin import credentials
from firebase_admin import firestore
from random import randint
app = Flask(__name__)
CORS(app)

cred = credentials.Certificate("serviceAccountKey.json")

collection = 'urls'
app2 = firebase_admin.initialize_app(cred)

db = firestore.client()
with open('remote_dns_server.txt', 'r') as file:
    DNS_EXTERNAL_SERVER=file.read().strip()

def process_geo(curr_doc, requested_ip_str):
    requested_ip = ipaddress.IPv4Address(requested_ip_str) #Transforma la ip a un tipo específico para compararla
    query_start = db.collection('country_ip').where('start_ip', '<=', str(requested_ip)).order_by('start_ip', direction=firestore.Query.DESCENDING).limit(100)
    #Se limita a 100 porque no se puede where en 2 campos diferentes y al comparar strings puede devolver el registro equivocado con 1
    result_start = query_start.get()

    # Aquí se guarda el id del país
    matching_id = None

    # Revisa cada fila devuelta
    for doc in result_start:
        doc_data = doc.to_dict()
        start_ip = ipaddress.IPv4Address(doc_data['start_ip'])
        end_ip = ipaddress.IPv4Address(doc_data['end_ip'])
        if start_ip <= requested_ip <= end_ip:
            matching_id = (doc_data['country']).lower() #Los países en la base de datos deben estar en minúscula
            if matching_id in curr_doc: #si el país está considerado retornar ese
                return curr_doc[matching_id]
            else: #sino retornar el default
                return curr_doc["default"]


def process_multi(curr_doc, ref_doc):
    index = curr_doc["index"]
    servers = curr_doc["servers"]
    new_index = int(index)+1
    ref_doc.update({"index": new_index})
    rr_index = index % len(servers)
    return servers[rr_index]

def process_weight(curr_doc):
    total_weight = 0
    rand_ind = randint(0,100)
    for curr_url in curr_doc["servers"]:
        total_weight+=curr_url["w"]
        if rand_ind<=total_weight:
            return curr_url["server"]
    return curr_doc["servers"][0]["server"]


def process_single(curr_doc):

    return curr_doc["server"]


# Endpoint to retrieve all data
@app.route('/api/dns_resolver', methods=['POST', 'GET'])
def dns_resolver():
    url = request.args.get('url')
    origin = request.args.get('src')
    requested = db.collection('urls').document(url)

    doc = requested.get()
    if doc.exists:
        print(f'Document data: {doc.to_dict()}')
        url_doc = doc.to_dict()
        if url_doc['type'] == 'geo':
            return process_geo(url_doc, origin)

        elif url_doc["type"] == "multi":
            return process_multi(url_doc, requested)

        elif url_doc["type"] == "weight":
            return process_weight(url_doc)
        elif url_doc["type"] == "single":
            return process_single(url_doc)
        else:
            return "Error"

    else:
        print('No such document!')
        return "No"

@app.route('/dns_request', methods=['POST'])
def dns_request():
    query_data_base64 = request.get_data()

    # Decodificar datos base64
    query_data = base64.b64decode(query_data_base64)

    #query_data = dns.message.from_wire(query_data)

    # Enviar query a server dns
    response = dns.query.udp(query_data, DNS_EXTERNAL_SERVER)

    # Codificar datos base64
    #response = response.to_wire()
    response_data_base64 = base64.b64encode(response).decode('utf-8')

    #return response_data_base64, 200, {'Content-Type': 'application/dns-message'}
    return response_data_base64



# Endpoint to retrieve all data
@app.route('/api/write_json', methods=['POST'])
def db_write():
    doc_ref = db.collection('urls').document('www.itcr.ac.cr')
    doc_ref.set({
        'servers': ["128.8.8.8", "129.9.9.9"],
        'index': 1,
        'reg_type': "multi"
    })
    return "Succesful"

@app.route('/api/write_json4', methods=['POST'])
def db_write4():
    doc_ref = db.collection('urls').document('www.uned.ac.cr')
    doc_ref.set({
        'server': "149.9.9.9",
        'reg_type': "single"
    })
    return "Succesful"


@app.route('/api/upload_dbip2', methods=['POST'])
def upload_CTIP_ranges():
    with open('dbip.csv', 'r') as file:
        csv_data = csv.reader(file)
        counter = 0
        line_number = 0
        #ranges = [(179810, 179910), (231000, 234700), (275290, 276290)]
        ranges = [(232106, 234700), (275290, 276290)]

        for row in csv_data:
            line_number += 1
            if line_number < ranges[0][0]:
                continue  # Skip lines before the first range

            if ranges and line_number > ranges[0][1]:
                ranges.pop(0)  # Remove the processed range

            if not ranges:
                break  # Stop processing when all ranges are processed

            counter += 1
            print(counter)
            start_ip = row[0]
            end_ip = row[1]
            countryid = row[2]

            start_ip = ipaddress.IPv4Address(start_ip)
            end_ip = ipaddress.IPv4Address(end_ip)

            doc_ref = db.collection('country_ip').document()
            doc_ref.set({
                'start_ip': str(start_ip),
                'end_ip': str(end_ip),
                'country': countryid
            })
    return "Success"



@app.route('/api/upload_dbip', methods=['POST'])
def upload_CTIP():
    with open('dbip.csv', 'r') as file:
        csv_data = csv.reader(file)
        counter =0
        # Recorre cada fila del csv
        for row in csv_data:
            counter+=1
            print(counter)
            start_ip = row[0]
            end_ip = row[1]
            countryid = row[2]


            # #Convierte las direcciones del rango a tipo ipaddress
            start_ip = ipaddress.IPv4Address(start_ip)
            end_ip = ipaddress.IPv4Address(end_ip)

            # Store the data in Firebase
            doc_ref = db.collection('country_ip').document()
            doc_ref.set({
                'start_ip': str(start_ip),
                'end_ip': str(end_ip),
                'country': countryid
            })
            time.sleep(1)
    return "Success"




@app.route('/api/write_json2', methods=['POST'])
def db_write2():
    doc_ref = db.collection('urls').document('www.ucr.ac.cr')
    doc_ref.set({
        'servers':[{'server': "189.9.9.9",
        'w': 10}, {'server': "188.9.9.9",
        'w': 90}],
        'reg_type': "weight"

    })
    return "Succesful"



@app.route('/api/write_json3', methods=['POST'])
def db_write3():
    doc_ref = db.collection('urls').document('www.una.ac.cr')
    doc_ref.set({
        'cr': "158.8.8.8",
        'uk': "159.9.9.9",
        'default': "157.9.9.9",
        'type': "geo"

    })
    return "Succesful"


@app.route('/getAll', methods=['GET'])
def get_all():


    try:
        collection_ref = db.collection(collection)
        documents = collection_ref.get()

        # Process the retrieved documents
        document_list = []
        for doc in documents:
            document_id = doc.id
            document_data = doc.to_dict()
            newDoc = {'id': document_id, 'data':document_data}
            document_list.append(newDoc)

        return jsonify({'success': True, 'documents': document_list})
    except Exception as e:
        return jsonify({'success': False, 'message': str(e)}), 500

@app.route('/get/<url>', methods=['GET'])
def getDocument(url):
    try:
        # Get a reference to the document
        doc_ref = db.collection(collection).document(url)

        # Retrieve the document
        doc = doc_ref.get()

        if doc.exists:
            document_data = doc.to_dict()
            return jsonify({'success': True, 'document': document_data})
        else:
            return jsonify({'success': False, 'message': 'Document not found'})
    except Exception as e:
        return jsonify({'success': False, 'message': str(e)}), 500


@app.route('/update', methods=['POST'])
def update():
    try:
        ## get url to update
        data = request.get_json()
        url = data['url']
        document = data['document']

        doc_ref = db.collection(collection).document(url)
        doc_ref.update(document)

        updated_doc = doc_ref.get()
        if updated_doc.exists:
            return jsonify({'success': True, 'message': 'Document updated successfully'})
        else:
            return jsonify({'success': False, 'message': 'Failed to update the document'})
    except Exception as e:
        return jsonify({'success': False, 'message': str(e)}), 500

@app.route('/new', methods=['PUT'])
def new():
    try:
        data = request.get_json()
        url = data['url']
        document = data['document']

        doc_ref = db.collection(collection).document(url)
        doc_ref.set(document)

        # Check if the document was created successfully
        created_doc = doc_ref.get()
        if created_doc.exists:
            return jsonify({'success': True, 'message': 'Document created successfully'})
        else:
            return jsonify({'success': False, 'message': 'Failed to create the document'})
    except Exception as e:
        return jsonify({'success': False, 'message': str(e)}), 500


@app.route('/del', methods=['DELETE'])
def delete():
    try:
        # Get data from request JSON
        data = request.get_json()
        url = data['url']

        # Get a reference to the document
        doc_ref = db.collection(collection).document(url)
        
        # Delete the document
        doc_ref.delete()

        # Check if the document was successfully deleted
        deleted_doc = doc_ref.get()
        if not deleted_doc.exists:
            return jsonify({'success': True, 'message': 'Document deleted successfully'})
        else:
            return jsonify({'success': False, 'message': 'Failed to delete the document'})
    except Exception as e:
        return jsonify({'success': False, 'message': str(e)}), 500


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)