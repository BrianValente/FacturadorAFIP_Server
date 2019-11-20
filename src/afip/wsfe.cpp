#include <iostream>
#include <vector>

#include "wsfe.hpp"

#include "../invoice.hpp"
#include "../config.hpp"

namespace Wsfe {

	ServiceSoapProxy* getSoapProxy() {
		ServiceSoapProxy* proxy = new ServiceSoapProxy("https://wswhomo.afip.gov.ar/wsfev1/service.asmx");
        //ServiceSoapProxy* proxy = new ServiceSoapProxy("https://servicios1.afip.gov.ar/wsfev1/service.asmx");

		proxy->soap->connect_timeout = 5;
		proxy->soap->connect_retry = 0;

		return proxy;
	}

	afip3__FEAuthRequest* getAuth(AccessCredentials* credentials = Login::getAccessCredentials()) {
		afip3__FEAuthRequest* auth = new afip3__FEAuthRequest();

		auth->Token = &credentials->token;
		auth->Sign = &credentials->sign;
		auth->Cuit = Config::getULL("cuit");

		return auth;
	}

	bool sendInvoice(Invoice* invoice) {
        ServiceSoapProxy* proxy = getSoapProxy();
        _afip3__FECAESolicitar requestRoot;
        afip3__FECAERequest requestBody;
        afip3__FECAECabRequest invoicesHeader;
        afip3__ArrayOfFECAEDetRequest invoicesContainer;
        vector<afip3__FECAEDetRequest*> invoicesVector;
        afip3__FECAEDetRequest requestInvoice;
        
        static _afip3__FECAESolicitarResponse response;
        
        invoice->number = getLastInvoice()->number + 1;
        
        invoicesVector.push_back(&requestInvoice);
        
        requestRoot.Auth = getAuth();
        requestRoot.FeCAEReq = &requestBody;
        
        requestBody.FeCabReq = &invoicesHeader;
        requestBody.FeDetReq = &invoicesContainer;
        
        invoicesContainer.FECAEDetRequest = invoicesVector;
        
        invoicesHeader.CantReg = 1;
        invoicesHeader.PtoVta = invoice->sellingPoint;
        invoicesHeader.CbteTipo = invoice->type;
        
        requestInvoice.Concepto = invoice->sellType;
        requestInvoice.DocTipo = invoice->docType;
        requestInvoice.DocNro = invoice->docNumber;
        
        requestInvoice.CbteDesde = invoice->number;
        requestInvoice.CbteHasta = invoice->number;
        requestInvoice.CbteFch = &invoice->date;
        
        if (invoice->sellType == 2 || invoice->sellType == 3) {
            requestInvoice.FchServDesde = &invoice->date;
            requestInvoice.FchServHasta = &invoice->date;
            requestInvoice.FchVtoPago = &invoice->date;
        }

        requestInvoice.ImpTotal = invoice->amount;
        requestInvoice.ImpTotConc = 0;
        requestInvoice.ImpNeto = invoice->amount;
        requestInvoice.ImpOpEx = 0;
        requestInvoice.ImpIVA = 0;
        requestInvoice.ImpTrib = 0;
        requestInvoice.MonId = &invoice->currency;
        requestInvoice.MonCotiz = invoice->currencyExchangeRate;
        
        proxy->FECAESolicitar(&requestRoot, response);
        
        if (proxy->soap->error) {
            cerr << "There was an error connecting to server:" << endl << endl;
            proxy->soap_stream_fault(std::cerr);
            cerr << endl << endl;
            invoice->status = Invoice::STATUS_ERROR;
            
            return false;
        }
        
        invoice->cae = atol(response.FECAESolicitarResult->FeDetResp->FECAEDetResponse[0]->CAE->c_str());
        invoice->status = invoice->cae != 0? Invoice::STATUS_SENT : Invoice::STATUS_ERROR;

		return true;
	}

	Invoice* getLastInvoice() {
		ServiceSoapProxy* proxy = getSoapProxy();
		_afip3__FECompUltimoAutorizado request;
		_afip3__FECompUltimoAutorizadoResponse response;
		afip3__FERecuperaLastCbteResponse* response2;
		Invoice* invoice;
        
		request.Auth = getAuth();
		request.PtoVta = 2;
		request.CbteTipo = 11;
        
		proxy->FECompUltimoAutorizado(&request, response);

		if (proxy->soap->error) {
			cerr << "There was an error connecting to server:" << endl << endl;
			proxy->soap_stream_fault(std::cerr);
			cerr << endl << endl;
			return NULL;
		}

		response2 = response.FECompUltimoAutorizadoResult;

		if (!response2 || response2->CbteNro == 0) {
			return NULL;
		}

		invoice = new Invoice();

		invoice->type = response2->CbteTipo;
		invoice->number = response2->CbteNro;
		invoice->sellingPoint = response2->PtoVta;

		return invoice;
	}

	bool testConnection(AccessCredentials* credentials) {
		ServiceSoapProxy* proxy = getSoapProxy();
		_afip3__FECompUltimoAutorizado request;
		_afip3__FECompUltimoAutorizadoResponse response;

		request.Auth = getAuth(credentials);
		request.PtoVta = 2;
		request.CbteTipo = 11;

		proxy->FECompUltimoAutorizado(&request, response);

		if (proxy->soap->error) {
			return false;
		}

        // Checks if CbteNro != NULL
		return response.FECompUltimoAutorizadoResult->CbteNro;
	}
}
